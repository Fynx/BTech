set (BTGame_SRCS
	BTGame.cpp
	LogWindow.cpp
	main.cpp
	SideBar.cpp
)

set (BTGame_HDRS
	BTGame.h
	LogWindow.h
	SideBar.h
)

qt5_wrap_cpp (BTGame_SRCS ${BTGame_HDRS})

add_executable (BTGame ${BTGame_SRCS})
target_link_libraries (BTGame ${Qt5Widgets_LIBRARIES} BTCommon)
