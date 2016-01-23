qt5_wrap_ui(forms
  locationDialog.ui
  locationWidget.ui
  mainWindow.ui
  newAccountDialog.ui
  postListPage.ui
  postDetailsPage.ui
  postWidget.ui
  voteWidget.ui
)

qt5_add_resources(resources
  emoji.qrc
)

add_executable(DesktopJodel
  main.cpp 
  client.cpp
  colorWidget.cpp
  emoji.cpp
  imageFetcher.cpp
  imageWidget.cpp
  location.cpp
  locationDialog.cpp
  locationWidget.cpp
  mainWindow.cpp
  newAccountDialog.cpp
  postListPage.cpp
  postDetailsPage.cpp
  post.cpp
  postWidget.cpp
  voteDirection.cpp
  voteWidget.cpp
  ${forms}
  ${resources}
)

target_link_libraries(DesktopJodel
  Qt5::Widgets
  Qt5::Network
  ${X11_LIBRARIES}
) 

install(TARGETS DesktopJodel DESTINATION bin)
