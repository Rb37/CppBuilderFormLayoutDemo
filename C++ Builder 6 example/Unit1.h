//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GridLayout.h"
#include "StackLayout.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TPanel *MainPanel;
    TPanel *StatusPanel;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TButton *ButtonA;
    TButton *ButtonB;
    TButton *ButtonC;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall ButtonCClick(TObject *Sender);
private:	// User declarations
    TGridLayout mainLayout;
    TStackLayout statusLayout;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 