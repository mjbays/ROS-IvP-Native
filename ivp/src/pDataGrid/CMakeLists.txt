#--------------------------------------------------------
# The CMakeLists.txt for:                     pDataGrid
# Author(s):                                Mark Snyder
#--------------------------------------------------------

SET(SRC
  DataGrid.cpp
  DataGrid_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pDataGrid ${SRC})

TARGET_LINK_LIBRARIES(pDataGrid
   ${MOOS_LIBRARIES}
   geometry
   mbutil
   apputil
   contacts
   ${SYSTEM_LIBS}
)

