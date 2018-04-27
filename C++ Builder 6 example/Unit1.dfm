object Form1: TForm1
  Left = 192
  Top = 124
  Width = 1305
  Height = 675
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 1289
    Height = 552
    Align = alClient
    TabOrder = 0
    object Panel1: TPanel
      Left = 136
      Top = 48
      Width = 185
      Height = 41
      Caption = 'Panel1'
      Color = clSilver
      TabOrder = 0
    end
    object Panel2: TPanel
      Left = 360
      Top = 48
      Width = 185
      Height = 41
      Caption = 'Panel2'
      Color = clYellow
      TabOrder = 1
    end
    object Panel3: TPanel
      Left = 560
      Top = 48
      Width = 185
      Height = 41
      Caption = 'Panel3'
      Color = clFuchsia
      TabOrder = 2
    end
    object Panel4: TPanel
      Left = 152
      Top = 112
      Width = 185
      Height = 41
      Caption = 'Panel4'
      Color = clLime
      TabOrder = 3
    end
    object Panel5: TPanel
      Left = 361
      Top = 112
      Width = 184
      Height = 41
      Caption = 'Panel5'
      Color = clRed
      TabOrder = 4
    end
    object Panel6: TPanel
      Left = 552
      Top = 104
      Width = 185
      Height = 41
      Caption = 'Panel6'
      Color = clWhite
      TabOrder = 5
    end
    object Panel7: TPanel
      Left = 144
      Top = 168
      Width = 185
      Height = 41
      Caption = 'Panel7'
      Color = clAqua
      TabOrder = 6
    end
    object Panel8: TPanel
      Left = 360
      Top = 168
      Width = 185
      Height = 41
      Caption = 'Panel8'
      Color = clTeal
      TabOrder = 7
    end
    object Panel9: TPanel
      Left = 568
      Top = 160
      Width = 185
      Height = 41
      Caption = 'Panel9'
      Color = clMaroon
      TabOrder = 8
    end
  end
  object StatusPanel: TPanel
    Left = 0
    Top = 552
    Width = 1289
    Height = 85
    Align = alBottom
    TabOrder = 1
    object ButtonA: TButton
      Left = 1128
      Top = 32
      Width = 75
      Height = 25
      Caption = 'ButtonA'
      TabOrder = 0
    end
    object ButtonB: TButton
      Left = 984
      Top = 40
      Width = 75
      Height = 25
      Caption = 'ButtonB'
      TabOrder = 1
    end
    object ButtonC: TButton
      Left = 888
      Top = 40
      Width = 75
      Height = 25
      Caption = 'ButtonC'
      TabOrder = 2
      OnClick = ButtonCClick
    end
  end
end
