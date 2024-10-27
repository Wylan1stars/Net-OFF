[Setup]
AppName=NetOff
AppVersion=1.0
DefaultDirName={pf}\Net-OFF
DefaultGroupName=Net-OFF
UninstallDisplayIcon={app}\NET-OFF.ico
OutputBaseFilename=Setup
SetupIconFile=C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\NET-OFF.ICO
WizardImageFile=C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\NET-OFF.bmp
PrivilegesRequired=admin

[Files]
Source: "C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\dist\app.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\dist\hora.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\dist\config.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\MX-PC.DESKTOP-9IJM2SP\Documents\NetOff_rehecho\dist\instrucciones.txt"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\NetOff"; Filename: "{app}\app.exe"; IconFilename: "{app}\NET-OFF.ico"
Name: "{commondesktop}\NetOff"; Filename: "{app}\app.exe"; IconFilename: "{app}\NET-OFF.ico"
Name: "{group}\Configurar Hora de Apagado"; Filename: "{app}\hora.exe"

[Run]
Filename: "{app}\hora.exe"; Description: "Configurar Hora de Apagado"; Flags: nowait postinstall
Filename: "{app}\app.exe"; Description: "Ejecutar en segundo plano"; Flags: nowait postinstall skipifsilent

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "ApagadoProgramado"; ValueData: """{app}\app.exe"""

[UninstallDelete]
Type: filesandordirs; Name: "{app}"

[Messages]
WelcomeLabel1=Bienvenido al asistente de instalación de ApagadoProgramado.
InfoBeforeLabel=Por favor, lea las siguientes instrucciones antes de continuar.

[CustomMessages]
ReadmeDesc=Instrucciones de uso:{#13}1. Abra 'Configurar Hora de Apagado' desde el menú Inicio o la carpeta de instalación.{#13}2. Ingrese la hora a la que desea que se apague el equipo y guárdela.{#13}3. El programa se ejecutará en segundo plano y apagará el equipo a la hora configurada.{#13}4. Para cambiar la hora de apagado, vuelva a ejecutar 'Configurar Hora de Apagado'.{#13}5. ¡No cierre el proceso en segundo plano si desea mantener la función activa!
