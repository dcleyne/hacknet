#include "HN_Random.h"
#include "OBJ_Manager.h"
#include "OBJ_Definition.h"
#include "OBJ_Base.h"
#include <stdio.h>

objBase::objBase(uint32 itemID) :
		m_itemID(itemID), m_next(this), m_prev(this), m_count(1), m_blesscurse(
				BC_Uncursed), m_flags(0)
{
	const objPrototype &proto = objManager::GetInstance()->GetPrototype(
			GetItemID());
	m_type = proto.type;

	m_position.Set(0, 0, 0);
}

objBase::~objBase()
{
}

sint16 objBase::RollDamage(entBase *foe)
{
	const objPrototype &proto = objManager::GetInstance()->GetPrototype(
			GetItemID());

	sint16 damage = 0;

	// we should inspect the foe to determine if it's big or
	// small, whether we do extra damage to it (due to it
	// being silver fearing and us being silver, etc.)

	if (0) // if the monster is big...
	{
		if (proto.largeDamage)
			damage = hnRandom::GetInstance()->GetAndAdd(proto.largeDamage, 1);
		damage += proto.largeDamageExtra;
	}
	else // if the monster is small...
	{
		if (proto.smallDamage)
			damage = hnRandom::GetInstance()->GetAndAdd(proto.smallDamage, 1);
		damage += proto.smallDamageExtra;
	}

	return damage;
}

const hnPoint& objBase::GetPosition()
{
	return m_position;
}

void objBase::SetPosition(const hnPoint & pos)
{
	m_position = pos;
}

bool objBase::SetWieldedPrimary(bool wielded)
{
	if (wielded)
		m_flags |= FLAG_WieldedPrimary;
	else
		m_flags &= ~FLAG_WieldedPrimary;

	return true; // TODO:  Return success/fail based upon whether stuff changed.
}

bool objBase::SetWieldedSecondary(bool wielded)
{
	if (wielded)
		m_flags |= FLAG_WieldedSecondary;
	else
		m_flags &= ~FLAG_WieldedSecondary;

	return true; // TODO:  Return success/fail based upon whether stuff changed.
}

bool objBase::SetWorn(bool worn)
{
	printf("Received wear request for something that can't be worn.\n");
	return false; // you can't wear anything but armour, rings, and some tools.
}

void objBase::AddObject(objBase * object)
{
	//---------------------------------------------------------
	//  We can't just innocently add the object to our list --
	//  since we're tracking object counts and stacking items
	//  properly, we need to actually check this new object
	//  against our current contents, and only add it if we
	//  don't have another object of identical itemID!
	//---------------------------------------------------------

	objBase *shuttle = m_next;

	while (shuttle != this)
	{
		if (shuttle->m_itemID == object->m_itemID
				&& shuttle->m_blesscurse == object->m_blesscurse)
		{
			// we're stackable, so just stack us.
			shuttle->m_count += object->m_count;
			delete object;
			return;
		}

		shuttle = shuttle->m_next;
	}
	//---------------------------------------------------------
	//  If we reach this point, we weren't stackable, so add
	//  us to the list.
	//---------------------------------------------------------

	object->m_prev = this;
	object->m_next = m_next;
	m_next->m_prev = object;
	m_next = object;
}

void objBase::RemoveObject(objBase * object)
{
	//---------------------------------------------------------
	//  Check the list.  If we find this object in our list,
	//  then remove it from the list.  This function probably
	//  will never actually be called.
	//---------------------------------------------------------

	objBase *shuttle = m_next;

	while (shuttle != this)
	{
		if (shuttle == object)
		{
			object->Unlink();
			break;
		}
		shuttle = shuttle->m_next;
	}
}

const char *
objBase::GetName()
{
	const objPrototype &proto = objManager::GetInstance()->GetPrototype(
			m_itemID);

	return proto.name;
}

objBase *
objBase::RemoveObjectQuantity(objBase *object, uint8 count)
{
	//---------------------------------------------------------
	//  Check the list.  If we find this object in our list,
	//  then remove the quantity specified as 'count'.
	//---------------------------------------------------------

	objBase *result = NULL;
	objBase *shuttle = m_next;

	while (shuttle != this)
	{
		if (shuttle == object)
		{
			if (object->m_count >= count)
			{
				RemoveObject(object);
				result = object;
			}
			else if (object->m_count < count)
			{
				object->m_count -= count;
				result = new objBase(object->m_itemID);
				result->m_count = count;
			}

			break;
		}
		shuttle = shuttle->m_next;
	}

	return result;
}

/*
 objBase *
 objBase::RemoveObjectDescription( const objDescription &desc, uint8 id )
 {
 objBase *object = GetObject(id);
 objBase *result = NULL;

 if ( object && object->PartialMatch(desc) )
 {
 if ( object->m_count == desc.count )
 {
 RemoveObject(object);
 result = object;
 }
 else if ( object->m_count < desc.count )
 {
 object->m_count -= desc.count;

 result = new objBase(object->m_itemID, hnPoint(0,0,0));
 }
 else
 printf("ERROR: Object quantity confusion failure in PartialMatch().\n");
 }

 return result;
 }
 */

void objBase::Unlink()
{
	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;

	m_next = m_prev = this;
}

bool objBase::ExactMatch(const objDescription &desc)
{
	bool matches = false;

	if (desc.itemID == m_itemID)
	{
		if (desc.blesscurse == BC_Unknown || desc.blesscurse == m_blesscurse)
		{
			if (desc.count == m_count && desc.flags == m_flags)
			{
				matches = true;
			}
		}
	}

	return matches;
}

bool objBase::PartialMatch(const objDescription &desc)
{
	bool matches = false;

	if (desc.itemID == m_itemID)
	{
		if (desc.blesscurse == BC_Unknown || desc.blesscurse == m_blesscurse)
		{
			if (desc.count <= m_count && desc.flags == m_flags)
			{
				matches = true;
			}
		}
	}

	return matches;
}

int objBase::ObjectCount()
{
	int count = 0;

	objBase *shuttle = m_next;

	while (shuttle != this)
	{
		count++;
		shuttle = shuttle->m_next;
	}

	return count;
}

objBase *
objBase::GetObject(int id)
{
	objBase *result = NULL;

	if (id < ObjectCount())
	{
		result = m_next;

		for (int i = 0; i < id; i++)
			result = result->m_next;
	}

	return result;
}

int objBase::GetObjectID(objBase * object)
{
	int result = -1;
	int searchID = 0;
	objBase *search = m_next;

	while (search != this && search != object)
	{
		searchID++;
		search = search->m_next;
	}

	if (search == object)
		result = searchID;

	return result;
}

signed char objBase::BlessCurseSign()
{
	signed char a, b;

	a = (Blessed()) ? 1 : 0;
	b = (Cursed()) ? 1 : 0;

	return a - b;
}

void objBase::GetDescription(objDescription &result, int id)
{
	objBase *object = GetObject(id);

	if (object)
		object->FillDescription(result);
}

void objBase::FillDescription(objDescription &desc)
{
	// const objPrototype &proto = objManager::GetInstance()->GetPrototype(m_itemID);

	desc.type = m_type;
	desc.itemID = m_itemID;
	desc.count = m_count;
	desc.blesscurse = m_blesscurse;
	desc.flags = m_flags;
}
