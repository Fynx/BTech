set (BTMapEditor_SRCS
	BTMapEditor.cpp
	ClickModeManager.cpp
	dialogs/AbstractEditMechsWidget.cpp
	dialogs/EditMechPartsWidget.cpp
	dialogs/EditMechsDialog.cpp
	dialogs/EditMechStatsWidget.cpp
	dialogs/EditWeaponsDialog.cpp
	dialogs/EditWeaponsWidget.cpp
	dialogs/ItemComboBox.cpp
	dialogs/NewMapDialog.cpp
	main.cpp
	ManagersUtils.cpp
	MapPropertiesManager.cpp
	TerrainManager.cpp
	ToolBar.cpp
	UnitsManager.cpp
)

set (BTMapEditor_HDRS
	BTMapEditor.h
	ClickModeManager.h
	dialogs/AbstractEditMechsWidget.h
	dialogs/EditMechPartsWidget.h
	dialogs/EditMechsDialog.h
	dialogs/EditMechStatsWidget.h
	dialogs/EditWeaponsDialog.h
	dialogs/EditWeaponsWidget.h
	dialogs/ItemComboBox.h
	dialogs/NewMapDialog.h
	ManagersUtils.h
	MapPropertiesManager.h
	TerrainManager.h
	ToolBar.h
	UnitsManager.h
)

qt5_wrap_cpp (BTMapEditor_SRCS ${BTMapEditor_HDRS})

add_executable (BTMEditor ${BTMapEditor_SRCS})
target_link_libraries (BTMEditor ${Qt5Widgets_LIBRARIES} BTCommon)
