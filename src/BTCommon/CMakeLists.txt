set (BTCommon_SRCS
	Action.cpp
	AttackObject.cpp
	BiHash.cpp
	BTMapManager.cpp
	Colors.cpp
	DataManager.cpp
	Effect.cpp
	EnumHashFunctions.h
	GraphicsEntity.cpp
	GraphicsFactory.cpp
	GraphicsHex.cpp
	GraphicsMap.cpp
	Grid.cpp
	Hex.cpp
	InfoBar.cpp
	Map.cpp
	Mech.cpp
	MechBase.cpp
	MechEntity.cpp
	MechPart.cpp
	MechPartBase.cpp
	MechWarrior.cpp
	MoveObject.cpp
	Objects.cpp
	Paths.cpp
	Player.cpp
	Position.cpp
	Rules.cpp
	Settings.cpp
	Utils.cpp
	Weapon.cpp
	WeaponBase.cpp
)

set (BTCommon_HDRS
	BTMapManager.h
	GraphicsEntity.h
	GraphicsHex.h
	GraphicsMap.h
	InfoBar.h
	MechBase.h
	MechEntity.h
	Objects.h
	Player.h
	Rules.h
	WeaponBase.h
)

qt5_wrap_cpp (BTCommon_SRCS ${BTCommon_HDRS})

add_library (BTCommon ${BTCommon_SRCS})
target_link_libraries (BTCommon ${Qt5Widgets_LIBRARIES})
