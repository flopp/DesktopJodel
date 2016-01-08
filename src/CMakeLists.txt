qt5_wrap_ui(forms
  mainWindow.ui
  newAccountDialog.ui
  postListPage.ui
  postDetailsPage.ui
  postWidget.ui
  voteWidget.ui
)

#qt5_add_resources(resources
#  src.qrc
#)

add_executable(DesktopJodel
  main.cpp 
  client.cpp
  colorWidget.cpp
  imageFetcher.cpp
  imageWidget.cpp
  mainWindow.cpp
  newAccountDialog.cpp
  postListPage.cpp
  postDetailsPage.cpp
  post.cpp
  postWidget.cpp
  voteDirection.cpp
  voteWidget.cpp
  ${forms}
)

target_link_libraries(DesktopJodel
  Qt5::Widgets
  Qt5::Network
  ${X11_LIBRARIES}
) 

install(TARGETS DesktopJodel DESTINATION bin)
