!define PRODUCT_NAME "Kiax"
!define PRODUCT_VERSION "2.0"
!define PRODUCT_ICON "installicon.ico"
!define INSTALLER_ICON "installicon.ico"
!define PRODUCT_PUBLISHER "Forschung-Direkt GmbH"
!define PRODUCT_WEB_SITE "www.kiax.org"
;!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\makensis.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PROTOCOL_NAME "voip"

!define MUI_ICON "${INSTALLER_ICON}"
!define MUI_UNICON "${INSTALLER_ICON}"

;за използване на 'Модерния' потребителски интерефейс
!include "MUI.nsh"

;ще покаже съобщение ако потребителя иска да прекрати инсталацията
!define MUI_ABORTWARNING

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
; !insertmacro MUI_PAGE_LICENSE "license.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\gui.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------


;====================================================

; The name of the installer
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"

; The file to write
OutFile "${PRODUCT_NAME}${PRODUCT_VERSION}install.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}${PRODUCT_VERSION}"

; The text to prompt the user to enter a directory
DirText "This will install ${PRODUCT_NAME}${PRODUCT_VERSION} Please choose a directory"

;--------------------------------

; What to install
Section "" ;No components page, name is not important

; Set output path to the installation directory.
SetOutPath $INSTDIR

File /r "release\*.*"


CreateDirectory "$INSTDIR\icons"
SetOutPath "$INSTDIR\icons"

File /r "release\icons\*.*"
SetOutPath $INSTDIR

CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\gui.exe"
CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\gui.exe"
CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\Uninstall.exe"


WriteRegStr HKCR "${PROTOCOL_NAME}" "" "URL:${PROTOCOL_NAME} Protocol"
WriteRegStr HKCR "${PROTOCOL_NAME}" "URL Protocol" ""
WriteRegStr HKCR "${PROTOCOL_NAME}\shell\open\command" "" "$INSTDIR\gui.exe %1"


; Tell the compiler to write an uninstaller and to look for a "Uninstall" section

WriteUninstaller $INSTDIR\Uninstall.exe


; WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\makensis.exe"

 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
; WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "${PRODUCT_ICON}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

SectionEnd ; end the section

; The uninstall section

Section "Uninstall"

RMDir /r "$INSTDIR"

DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
Delete "$SMPROGRAMS\${PRODUCT_NAME}\*.*"
Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"

SectionEnd 


