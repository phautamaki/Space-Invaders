Spawnable entities can be found in CEntity.h, they are also listed here
(might not be updated). When making changes to levelXinfo.txt make sure
you follow these marking rules.

Level change:
- The last marking in the .txt, first number tells in which point X
the change occurs

Subtypes:
- Different bullets start from number 1
- Different enemies start from number 100
- Different items start from number 1000

enum Types {
	ENTITY_TYPE_GENERIC = 0,
	ENTITY_TYPE_PLAYER = 1,
	ENTITY_TYPE_ENEMY = 2,
	ENTITY_TYPE_ITEM = 3,
	ENTITY_TYPE_BULLET = 4,
	ENTITY_TYPE_SPECIAL_EFFECT = 5
};
enum SubTypes {
	ENTITY_SUBTYPE_NONE = 0,

	ENTITY_SUBTYPE_BULLET_NORMAL = 1,
	ENTITY_SUBTYPE_BULLET_SMALL_45U = 2,
	ENTITY_SUBTYPE_BULLET_SMALL_45D = 3,
	ENTITY_SUBTYPE_BULLET_CHARGE1 = 4,
	ENTITY_SUBTYPE_BULLET_BEAM = 4,

	ENTITY_SUBTYPE_ENEMY_1 = 100,
	//ENTITY_SUBTYPE_ENEMY_2 = 101,
	//ENTITY_SUBTYPE_ENEMY_3 = 102,

	ENTITY_SUBTYPE_ITEM_1 = 1000,
	ENTITY_SUBTYPE_ITEM_2 = 1001
	//ENTITY_SUBTYPE_ITEM_3 = 1002

};