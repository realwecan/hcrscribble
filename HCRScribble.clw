; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHCRScribbleView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HCRScribble.h"
LastPage=0

ClassCount=6
Class1=CHCRScribbleApp
Class2=CHCRScribbleDoc
Class3=CHCRScribbleView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_HCRSCRTYPE
Class5=CAboutDlg
Class6=CChildFrame
Resource4=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource5=IDR_MAINFRAME (Chinese (P.R.C.))
Resource6=IDR_HCRSCRTYPE (Chinese (P.R.C.))

[CLS:CHCRScribbleApp]
Type=0
HeaderFile=HCRScribble.h
ImplementationFile=HCRScribble.cpp
Filter=N

[CLS:CHCRScribbleDoc]
Type=0
HeaderFile=HCRScribbleDoc.h
ImplementationFile=HCRScribbleDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CHCRScribbleDoc

[CLS:CHCRScribbleView]
Type=0
HeaderFile=HCRScribbleView.h
ImplementationFile=HCRScribbleView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_863_LOAD

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M

[CLS:CAboutDlg]
Type=0
HeaderFile=HCRScribble.cpp
ImplementationFile=HCRScribble.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
CommandCount=9

[MNU:IDR_HCRSCRTYPE]
Type=1
Class=CHCRScribbleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_RECO_RECOGNIZE
Command14=ID_863_TRAIN
Command15=ID_863_LOAD
Command16=ID_863_TEST
Command17=ID_MENUITEM32790
Command18=ID_MENUITEM32791
Command19=ID_MENUITEM32793
Command20=ID_MENUITEM32799
Command21=ID_MENUITEM32777
Command22=ID_MENUITEM32778
Command23=ID_OPTIONS_IDIRCH
Command24=ID_OPTIONS_DIRCH
Command25=ID_OPTIONS_8DIR
Command26=ID_MENUITEM32782
Command27=ID_MENUITEM32783
Command28=ID_MENUITEM32795
Command29=ID_MENUITEM32796
Command30=ID_MENUITEM32797
Command31=ID_MENUITEM32784
Command32=ID_WINDOW_NEW
Command33=ID_WINDOW_CASCADE
Command34=ID_WINDOW_TILE_HORZ
Command35=ID_WINDOW_ARRANGE
Command36=ID_HELP_FINDER
Command37=ID_APP_ABOUT
CommandCount=37

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[TB:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
CommandCount=9

[MNU:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_HCRSCRTYPE (Chinese (P.R.C.))]
Type=1
Class=CHCRScribbleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_RECO_RECOGNIZE
Command14=ID_863_TRAIN
Command15=ID_863_LOAD
Command16=ID_863_TEST
Command17=ID_MENUITEM32790
Command18=ID_MENUITEM32791
Command19=ID_MENUITEM32793
Command20=ID_MENUITEM32799
Command21=ID_MENUITEM32777
Command22=ID_MENUITEM32778
Command23=ID_OPTIONS_IDIRCH
Command24=ID_OPTIONS_DIRCH
Command25=ID_OPTIONS_8DIR
Command26=ID_MENUITEM32782
Command27=ID_MENUITEM32783
Command28=ID_MENUITEM32795
Command29=ID_MENUITEM32796
Command30=ID_MENUITEM32797
Command31=ID_MENUITEM32784
Command32=ID_WINDOW_NEW
Command33=ID_WINDOW_CASCADE
Command34=ID_WINDOW_TILE_HORZ
Command35=ID_WINDOW_ARRANGE
Command36=ID_HELP_FINDER
Command37=ID_APP_ABOUT
CommandCount=37

[ACL:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

