replace ..\..\KIT.Engine\Configuration.xml /a

xcopy ..\..\KIT.Engine\Core staging\Core /y /i /d /e /h
xcopy Configuration.xml staging\ /y /d
xcopy ..\..\KIT.Engine\usb.ids staging\ /y /d
xcopy ..\..\Workspace\Output\bin\Release\*.dll staging\ /y /d
xcopy ..\..\Workspace\Output\bin\Release\*.exe staging\ /y /d
xcopy Content staging\Content /y /i /d /e /h
xcopy Repository staging\Repository /y /i /d /e /h

xcopy ..\..\KIT.Engine\Core staging-debug\Core /y /i /d /e /h
xcopy Configuration.xml staging-debug\ /y /d
xcopy ..\..\KIT.Engine\usb.ids staging-debug\ /y /d
xcopy ..\..\Workspace\Output\bin\Debug\*.dll staging-debug\ /y /d
xcopy ..\..\Workspace\Output\bin\Debug\*.exe staging-debug\ /y /d
xcopy Content staging-debug\Content /y /i /d /e /h
xcopy Repository staging-debug\Repository /y /i /d /e /h
