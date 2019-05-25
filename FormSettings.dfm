object frmSettings: TfrmSettings
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Settings'
  ClientHeight = 251
  ClientWidth = 444
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBottom: TPanel
    Left = 0
    Top = 214
    Width = 444
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object btnCancel: TButton
      Left = 364
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object btnApply: TButton
      Left = 283
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Apply'
      TabOrder = 0
      OnClick = btnApplyClick
    end
  end
  object pages: TPageControl
    Left = 0
    Top = 0
    Width = 444
    Height = 214
    ActivePage = tsScripts
    Align = alClient
    TabOrder = 0
    object tsGeneral: TTabSheet
      Caption = 'General'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 333
      ExplicitHeight = 0
      object chbAlwaysOnTop: TCheckBox
        Left = 3
        Top = 3
        Width = 325
        Height = 17
        Caption = 'Window always on top'
        TabOrder = 0
        OnClick = chbAlwaysOnTopClick
      end
    end
    object tsScripts: TTabSheet
      Caption = 'Scripts'
      ImageIndex = 2
      object lblScriptsOnLoad: TLabel
        Left = 3
        Top = 3
        Width = 80
        Height = 13
        Caption = 'on load/call start'
      end
      object Label1: TLabel
        Left = 3
        Top = 71
        Width = 47
        Height = 13
        Caption = 'on unload'
      end
      object memoScriptsOnLoad: TMemo
        Left = 89
        Top = 3
        Width = 344
        Height = 62
        ScrollBars = ssVertical
        TabOrder = 0
      end
      object memoScriptsOnUnload: TMemo
        Left = 89
        Top = 71
        Width = 344
        Height = 62
        ScrollBars = ssVertical
        TabOrder = 1
      end
    end
  end
end
