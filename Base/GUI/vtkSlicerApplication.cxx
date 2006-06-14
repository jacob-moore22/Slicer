#include <sstream>
#include "vtkObjectFactory.h"
#include "vtkSlicerApplication.h"
#include "vtkSlicerGUICollection.h"
#include "vtkSlicerModuleGUI.h"
#include "vtkKWNotebook.h"
#include "vtkKWFrame.h"
#include "vtkKWUserInterfacePanel.h"
#include "vtkKWFrameWithLabel.h"
#include "vtkKWWindowBase.h"

const char *vtkSlicerApplication::ModulePathRegKey = "ModulePath";


//---------------------------------------------------------------------------
vtkStandardNewMacro (vtkSlicerApplication);
vtkCxxRevisionMacro(vtkSlicerApplication, "$Revision: 1.0 $");


//---------------------------------------------------------------------------
vtkSlicerApplication::vtkSlicerApplication ( ) {

    strcpy(this->ModulePath, "");

    // configure the application before creating
    this->SetName ( "3D Slicer Version 3.0 Alpha" );
    this->RestoreApplicationSettingsFromRegistry ( );
    this->SetHelpDialogStartingPage ( "http://www.slicer.org" );

    this->ModuleGUICollection = vtkSlicerGUICollection::New ( );
    vtkKWFrameWithLabel::SetDefaultLabelFontWeightToNormal( );
    this->MainLayout = vtkSlicerGUILayout::New ( );
    this->SlicerTheme = vtkSlicerTheme::New ( );

}



//---------------------------------------------------------------------------
vtkSlicerApplication::~vtkSlicerApplication ( ) {

    if ( this->MainLayout )
        {
            this->MainLayout->Delete ( );
            this->MainLayout = NULL;
        }
    if ( this->SlicerTheme )
        {
            this->SlicerTheme->Delete ( );
            this->SlicerTheme = NULL;
        }
    if ( this->ModuleGUICollection )
        {
            this->ModuleGUICollection->RemoveAllItems ( );
            this->ModuleGUICollection->Delete ( );
            this->ModuleGUICollection = NULL;
        }
}



//---------------------------------------------------------------------------
void vtkSlicerApplication::AddModuleGUI ( vtkSlicerModuleGUI *gui ) {

    // Create if it doesn't exist already
    if ( this->ModuleGUICollection == NULL ) {
        this->ModuleGUICollection = vtkSlicerGUICollection::New ( );
    } 
    // Add a gui
    this->ModuleGUICollection->AddItem ( gui );
}


//---------------------------------------------------------------------------
vtkSlicerModuleGUI* vtkSlicerApplication::GetModuleGUIByName ( const char *name )
{
    if ( this->ModuleGUICollection != NULL ) {
        int n = this->ModuleGUICollection->GetNumberOfItems ( );
        int i;
        for (i = 0; i < n; i ++ ) {
            vtkSlicerModuleGUI *m = vtkSlicerModuleGUI::SafeDownCast(
              this->ModuleGUICollection->GetItemAsObject(i) );
            if ( !strcmp (m->GetGUIName(), name) ) {
                return (m);
            }
        }
    }
    return ( NULL );
}


//---------------------------------------------------------------------------
void vtkSlicerApplication::ConfigureApplication ( ) {

    this->PromptBeforeExitOn ( );
    this->SupportSplashScreenOn ( );
    this->SplashScreenVisibilityOn ( );
    this->SaveUserInterfaceGeometryOn ( );
}



//---------------------------------------------------------------------------
void vtkSlicerApplication::InstallTheme ( vtkKWTheme *theme )
{
    if ( theme != NULL ) {
        if ( vtkSlicerTheme::SafeDownCast (theme) == this->SlicerTheme ) {
            this->SetTheme (this->SlicerTheme );
        } else {
            this->SetTheme ( theme );
        }
    }
}




//---------------------------------------------------------------------------
void vtkSlicerApplication::CloseAllWindows ( ) {
    int n, i;
    vtkKWWindowBase *win;
    
    n= this->GetNumberOfWindows ( );
    for (i=0; i<n; i++) {
        win = this->GetNthWindow ( n );
        win->Close ( );
    }
}


//---------------------------------------------------------------------------
int vtkSlicerApplication::StartApplication ( ) {

    int ret = 0;

    // Start the application & event loop here
    this->Start ( );
    this->CloseAllWindows ( );

    // Clean up and exit
    ret = this->GetExitStatus ( );
    return ret;
}


//----------------------------------------------------------------------------
void vtkSlicerApplication::RestoreApplicationSettingsFromRegistry()
{
  Superclass::RestoreApplicationSettingsFromRegistry();
  
  if (this->HasRegistryValue(
    2, "RunTime", vtkSlicerApplication::ModulePathRegKey))
    {
    this->GetRegistryValue(
      2, "RunTime", vtkSlicerApplication::ModulePathRegKey,
      this->ModulePath);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerApplication::SaveApplicationSettingsToRegistry()
{ 
  Superclass::SaveApplicationSettingsToRegistry();

  this->SetRegistryValue(
    2, "RunTime", vtkSlicerApplication::ModulePathRegKey, "%s", 
    this->ModulePath);
}


void vtkSlicerApplication::SetModulePath(const char* path)
{
  if (strcmp(this->ModulePath, path) != 0
      && strlen(path) < vtkKWRegistryHelper::RegistryKeyValueSizeMax)
    {
    strcpy(this->ModulePath, path);
    this->Modified();
    }
}

const char* vtkSlicerApplication::GetModulePath() const
{
  return this->ModulePath;
}
