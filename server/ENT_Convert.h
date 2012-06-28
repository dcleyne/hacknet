/*
 * ENT_Convert.h
 *
 *  Created on: Jun 28, 2012
 *      Author: DCleyne
 */

#ifndef ENT_CONVERT_H_
#define ENT_CONVERT_H_

// Alignment
#define A_NONE		Align_None
#define A_CHAOTIC	Align_Chaotic
#define A_NEUTRAL	Align_Neutral
#define A_LAWFUL	Align_Lawful

//Genocide Flags
#define G_UNIQ		Geno_Unique
#define G_NOHELL	Geno_NoHell
#define G_HELL		Geno_Hell
#define G_NOGEN		Geno_NoGenerate
#define G_SGROUP	Geno_SmallGroup
#define G_LGROUP	Geno_LargeGroup
#define G_GENO		Geno_Genocidable
#define G_NOCORPSE	Geno_NoCorpse

// Magic Resistance
#define MR_FIRE		Resist_Fire
#define MR_COLD		Resist_Cold
#define MR_SLEEP	Resist_Sleep
#define MR_DISINT	Resist_Disintegrate
#define MR_ELEC		Resist_Electricity
#define MR_POISON	Resist_Poison
#define MR_ACID		Resist_Acid
#define MR_STONE	Resist_Stone

// Attack Type
#define AT_NONE	Attack_None
#define AT_CLAW	Attack_Claw
#define AT_BITE	Attack_Bite
#define AT_KICK	Attack_Kick
#define AT_BUTT	Attack_Butt
#define AT_TUCH	Attack_Touch
#define AT_STNG	Attack_Sting
#define AT_HUGS	Attack_Hugs
#define AT_SPIT	Attack_Spit
#define AT_ENGL	Attack_Engulf
#define AT_BREA	Attack_Breath
#define AT_EXPL	Attack_Explodes
#define AT_BOOM	Attack_Boom
#define AT_GAZE	Attack_Gaze
#define AT_TENT	Attack_Tentacles
#define AT_WEAP	Attack_Weapon
#define AT_MAGC	Attack_Magic

// Attack Damage
#define AD_PHYS	Damage_Physical
#define AD_MAGM	Damage_MagicMissile
#define AD_FIRE	Damage_Fire
#define AD_COLD	Damage_Cold
#define AD_SLEE	Damage_Sleep
#define AD_DISN	Damage_Disintegration
#define AD_ELEC	Damage_Electric
#define AD_DRST	Damage_DrainStrength
#define AD_ACID	Damage_Acid
#define AD_SPC1	Damage_Special1
#define AD_SPC2	Damage_Special2
#define AD_BLND	Damage_Blind
#define AD_STUN	Damage_Stun
#define AD_SLOW	Damage_Slow
#define AD_PLYS	Damage_Paralyse
#define AD_DRLI	Damage_DrainLife
#define AD_DREN	Damage_DrainEnergy
#define AD_LEGS	Damage_Legs
#define AD_STON	Damage_Stone
#define AD_STCK	Damage_Stick
#define AD_SGLD	Damage_StealGold
#define AD_SITM	Damage_StealItem
#define AD_SEDU	Damage_Seduce
#define AD_TLPT	Damage_Teleport
#define AD_RUST	Damage_Rust
#define AD_CONF	Damage_Confuse
#define AD_DGST	Damage_Digest
#define AD_HEAL	Damage_Heal
#define AD_WRAP	Damage_Wrap
#define AD_WERE	Damage_Lycanthropy
#define AD_DRDX	Damage_DrainDexterity
#define AD_DRCO	Damage_DrainConstitution
#define AD_DRIN	Damage_DrainIntelligence
#define AD_DISE	Damage_Disease
#define AD_DCAY	Damage_Decay
#define AD_SUSD	Damage_SuccubusSeduction
#define AD_HALU	Damage_Hallucination
#define AD_DETH	Damage_Death
#define AD_PEST	Damage_Pestilence
#define AD_FAMN	Damage_Famine
#define AD_SLIM	Damage_Slime
#define AD_ENCH	Damage_RemoveEnchantment
#define AD_CORR	Damage_Corrode
#define AD_CLRC	Damage_RandomCleric
#define AD_SPEL	Damage_RandomSpell
#define AD_RBRE	Damage_RandomBreath
#define AD_SAMU	Damage_Samurai
#define AD_CURS	Damage_Curse

// Sounds
#define MS_SILENT 		Sound_Silent
#define MS_BARK			Sound_Bark
#define MS_MEW			Sound_Mew
#define MS_ROAR			Sound_Roar
#define MS_GROWL		Sound_Growl
#define MS_SQEEK		Sound_Squeak
#define MS_SQAWK		Sound_Squawk
#define MS_HISS			Sound_Hiss
#define MS_BUZZ			Sound_Buzz
#define MS_GRUNT		Sound_Grunt
#define MS_NEIGH		Sound_Neigh
#define MS_WAIL			Sound_Wail
#define MS_GURGLE		Sound_Gurgle
#define MS_BURBLE		Sound_Burble
#define MS_ANIMAL		Sound_Animal
#define MS_SHRIEK		Sound_Shriek
#define MS_BONES		Sound_Bones
#define MS_LAUGH		Sound_Laugh
#define MS_MUMBLE		Sound_Mumble
#define MS_IMITATE		Sound_Imitate
#define MS_ORC			Sound_Orc
#define MS_HUMANOID		Sound_Humanoid
#define MS_ARREST		Sound_Arrest
#define MS_SOLDIER		Sound_Soldier
#define MS_GUARD		Sound_Guard
#define MS_DJINNI		Sound_Djinni
#define MS_NURSE		Sound_Nurse
#define MS_SEDUCE		Sound_Seduce
#define MS_VAMPIRE		Sound_Vampire
#define MS_BRIBE		Sound_Bribe
#define MS_CUSS			Sound_Cuss
#define MS_RIDER		Sound_Rider
#define MS_LEADER		Sound_Leader
#define MS_NEMESIS		Sound_Nemesis
#define MS_GUARDIAN		Sound_Guardian
#define MS_SELL			Sound_Sell
#define MS_ORACLE		Sound_Oracle
#define MS_PRIEST		Sound_Priest
#define MS_SPELL		Sound_Spell
#define MS_WERE			Sound_Lycanthrope
#define MS_BOAST		Sound_Boast

// Size
#define MZ_TINY		Size_Tiny
#define MZ_SMALL	Size_Small
#define MZ_MEDIUM	Size_Medium
#define MZ_HUMAN	Size_Human
#define MZ_LARGE	Size_Large
#define MZ_HUGE		Size_Huge
#define MZ_GIGANTIC	Size_Gigantic

// Attributes
#define M1_FLY				M1_Fly
#define M1_SWIM				M1_Swim
#define M1_AMORPHOUS		M1_Amorphous
#define M1_WALLWALK			M1_WallWalk
#define M1_CLING			M1_Cling
#define M1_TUNNEL			M1_Tunnel
#define M1_NEEDPICK			M1_NeedPick
#define M1_CONCEAL			M1_Conceal
#define M1_HIDE				M1_Hide
#define M1_AMPHIBIOUS		M1_Amphibious
#define M1_BREATHLESS		M1_Breathless
#define M1_NOTAKE			M1_NoTake
#define M1_NOEYES			M1_NoEyes
#define M1_NOHANDS			M1_NoHands
#define M1_NOLIMBS			M1_NoLimbs
#define M1_NOHEAD			M1_NoHead
#define M1_MINDLESS			M1_Mindless
#define M1_HUMANOID			M1_Humanoid
#define M1_ANIMAL			M1_Animal
#define M1_SLITHY			M1_Slithery
#define M1_UNSOLID			M1_Unsolid
#define M1_THICK_HIDE		M1_ThickHide
#define M1_OVIPAROUS		M1_Oviparous
#define M1_REGEN			M1_Regenerate
#define M1_SEE_INVIS		M1_SeeInvisible
#define M1_TPORT			M1_Teleport
#define M1_TPORT_CNTRL		M1_TeleportControl
#define M1_ACID				M1_Acid
#define M1_POIS				M1_Poison
#define M1_CARNIVORE		M1_Carnivore
#define M1_HERBIVORE		M1_Herbivore
#define M1_OMNIVORE			M1_Omnivore
#define M1_METALLIVORE		M1_Metallivore



#define	M2_NOPOLY	 	M2_NoPolymorph
#define	M2_UNDEAD		M2_Undead
#define	M2_WERE			M2_Lycanthrope
#define	M2_HUMAN		M2_Human
#define	M2_ELF			M2_Elf
#define	M2_DWARF		M2_Dwarf
#define	M2_GNOME		M2_Gnome
#define	M2_ORC			M2_Orc
#define	M2_DEMON		M2_Demon
#define	M2_MERC			M2_Mercenary
#define	M2_LORD			M2_Lord
#define	M2_PRINCE		M2_Prince
#define	M2_MINION		M2_Minion
#define	M2_GIANT		M2_Giant
#define	M2_MALE			M2_Male
#define	M2_FEMALE		M2_Female
#define	M2_NEUTER		M2_Neuter
#define	M2_PNAME		M2_ProperName
#define	M2_HOSTILE		M2_Hostile
#define	M2_PEACEFUL		M2_Peaceful
#define	M2_DOMESTIC		M2_Domestic
#define	M2_WANDER		M2_Wander
#define	M2_STALK		M2_Stalk
#define	M2_NASTY		M2_Nasty
#define	M2_STRONG		M2_Strong
#define	M2_ROCKTHROW	M2_RockThrow
#define	M2_GREEDY		M2_Greedy
#define	M2_JEWELS		M2_Jewels
#define	M2_COLLECT		M2_Collect
#define	M2_MAGIC		M2_Magic



#define	M3_WANTSAMUL		M3_WantsAmulet
#define	M3_WANTSBELL		M3_WantsBell
#define	M3_WANTSBOOK		M3_WantsBook
#define	M3_WANTSCAND		M3_WantsCandelabrum
#define	M3_WANTSARTI		M3_WantsArtifact
#define	M3_WANTSALL			M3_WantsAll
#define	M3_WAITFORU			M3_WaitForYou
#define	M3_CLOSE			M3_Close
#define	M3_COVETOUS			M3_Covetous
#define	M3_WAITMASK			M3_WaitMask
#define	M3_INFRAVISION		M3_Infravision
#define	M3_INFRAVISIBLE		M3_Infravisable


// Entity Type
#define S_NONE				Entity_None
#define S_ANT				Entity_Ant
#define S_BLOB				Entity_Blob
#define S_COCKATRICE		Entity_Cockatrice
#define S_DOG				Entity_Dog
#define S_EYE				Entity_Eye
#define S_FELINE			Entity_Feline
#define S_GREMLIN			Entity_Gremlin
#define S_HUMANOID			Entity_Humanoid
#define S_IMP				Entity_Imp
#define S_JELLY				Entity_Jelly
#define S_KOBOLD			Entity_Kobold
#define S_LEPRECHAUN		Entity_Leprechaun
#define S_MIMIC				Entity_Mimic
#define S_NYMPH				Entity_Nymph
#define S_ORC				Entity_Orc
#define S_PIERCER			Entity_Piercer
#define S_QUADRUPED			Entity_Quadruped
#define S_RODENT			Entity_Rodent
#define S_SPIDER			Entity_Spider
#define S_TRAPPER			Entity_Trapper
#define S_UNICORN			Entity_Unicorn
#define S_VORTEX			Entity_Vortex
#define S_WORM				Entity_Worm
#define S_XAN				Entity_Xan
#define S_LIGHT				Entity_Light
#define S_ZRUTY				Entity_Zruty
#define S_ANGEL				Entity_Angel
#define S_BAT				Entity_Bat
#define S_CENTAUR			Entity_Centaur
#define S_DRAGON			Entity_Dragon
#define S_ELEMENTAL			Entity_Elemental
#define S_FUNGUS			Entity_Fungus
#define S_GNOME				Entity_Gnome
#define S_GIANT				Entity_Giant
#define S_JABBERWOCK		Entity_Jabberwock
#define S_KOP				Entity_Kop
#define S_LICH				Entity_Lich
#define S_MUMMY				Entity_Mummy
#define S_NAGA				Entity_Naga
#define S_OGRE				Entity_Ogre
#define S_PUDDING			Entity_Pudding
#define S_QUANTMECH			Entity_QuantumMechanic
#define S_RUSTMONST			Entity_RustMonster
#define S_SNAKE				Entity_Snake
#define S_TROLL				Entity_Troll
#define S_UMBER				Entity_UmberHulk
#define S_VAMPIRE			Entity_Vampire
#define S_WRAITH			Entity_Wraith
#define S_XORN				Entity_Xorn
#define S_YETI				Entity_Yeti
#define S_ZOMBIE			Entity_Zombie
#define S_GOLEM				Entity_Golem
#define S_HUMAN				Entity_Human
#define S_GHOST				Entity_Ghost
#define S_DEMON				Entity_Demon
#define S_EEL				Entity_Eel
#define S_LIZARD			Entity_Lizard
#define S_WORM_TAIL			Entity_WormTail
#define S_MAX				Entity_Type_Max

/*  Colors */
#define HI_OBJ			CLR_MAGENTA
#define HI_COPPER		CLR_YELLOW
#define HI_SILVER		CLR_SILVER
#define HI_CLOTH		CLR_BROWN
#define HI_ORGANIC		CLR_BROWN
#define HI_GLASS		CLR_BRIGHT_CYAN
#define HI_MINERAL		CLR_GRAY
#define HI_PAPER		CLR_WHITE
#define HI_WOOD			CLR_BROWN
#define HI_GOLD			CLR_YELLOW
#define HI_LEATHER		CLR_BROWN
#define HI_METAL		CLR_CYAN
#define HI_ZAP			CLR_BRIGHT_BLUE

#endif /* ENT_CONVERT_H_ */
