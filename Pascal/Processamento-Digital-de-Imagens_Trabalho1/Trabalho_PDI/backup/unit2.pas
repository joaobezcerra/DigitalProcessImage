unit Unit2;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,Math;

type

  { TForm2 }

  TForm2 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Edit5: TEdit;
    Edit6: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    StaticText1: TStaticText;
    StaticText2: TStaticText;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Label2Click(Sender: TObject);
    procedure Label3Click(Sender: TObject);
    procedure RadioButton1Change(Sender: TObject);
    procedure RadioButton2Change(Sender: TObject);
    procedure StaticText1Click(Sender: TObject);
  private

  public

  end;

var
  Form2: TForm2;

implementation

{$R *.lfm}

{ TForm2 }

//Botão RGB->HSV
procedure TForm2.RadioButton1Change(Sender: TObject);
begin
  if RadioButton1.Checked then
     begin
          Label3.Caption := 'R:';
          Label4.Caption := 'G:';
          Label5.Caption := 'B:';
          Label6.Caption := 'H:';
          Label7.Caption := 'S:';
          Label8.Caption := 'V:';
     end
  else
      begin
          Label3.Caption := 'H:';
          Label4.Caption := 'S:';
          Label5.Caption := 'V:';
          Label6.Caption := 'R:';
          Label7.Caption := 'G:';
          Label8.Caption := 'B:';
      end;

end;

procedure TForm2.Label2Click(Sender: TObject);
begin

end;

procedure TForm2.Label3Click(Sender: TObject);
begin

end;

procedure TForm2.FormCreate(Sender: TObject);
begin

end;
 //Botão Reset
procedure TForm2.Button1Click(Sender: TObject);
begin
  Edit1.Text := '';
  Edit2.Text := '';
  Edit3.Text := '';
  Edit4.Text := '';
  Edit5.Text := '';
  Edit6.Text := '';

  RadioButton1.Checked := True;
end;

//FUNÇÂO AUXILIAR, NÂO APRECE NA INTERFACE
function Min3(a, b, c: Double): Double;
begin
  Result := a;
  if b < Result then Result := b;
  if c < Result then Result := c;
end;

//FUNÇÂO AUXILIAR, NÂO APRECE NA INTERFACE
function Max3(a, b, c: Double): Double;
begin
  Result := a;
  if b > Result then Result := b;
  if c > Result then Result := c;
end;

//RGB PARA HSV
procedure RgbToHsv(r, g, b: Double; out h, s, v: Double);
var
  min, max, delta: Double;
begin
  min:= Min3(r,g,b);
  max:= Max3(r,g,b);
  v:= max; //o valor de v é igual o maior canal de RGB

  delta := max-min;

  if max = 0 then //isso verifica se não é a cor preta
     begin
          h:= 0;
          s:= 0;
          Exit;
     end;

  s:= delta/max;

  if delta = 0 then  //Se for cinza
     h:= 0
  else if r = max then
     h:= 60* ((g-b)/delta)
  else if g = max then
     h:= 60*(2+(b-r)/delta)
  else
    h:= 60*(4+(r-g)/delta);

  if h<0 then
     h:= h + 360;
end;

//HSV -> RGB
procedure HSVtoRGB(out r, g, b: Double; h, s, v: Double);
var
  i: Integer;
  f, p, q, t: Double;
begin
  if s = 0 then
  begin
    r := v;
    g := v;
    b := v;
    Exit;
  end;

  h := h / 60;
  i := Trunc(h);
  f := h - i;

  p := v * (1 - s);
  q := v * (1 - s * f);
  t := v * (1 - s * (1 - f));

  case i mod 6 of
    0: begin r := v; g := t; b := p; end;
    1: begin r := q; g := v; b := p; end;
    2: begin r := p; g := v; b := t; end;
    3: begin r := p; g := q; b := v; end;
    4: begin r := t; g := p; b := v; end;
    5: begin r := v; g := p; b := q; end;
  end;
end;

  //Botão Converter
procedure TForm2.Button2Click(Sender: TObject);
var
  r, g, b: Double;
  h, s, v: Double;
  isValid: Boolean;
begin
  isValid := True;

  try
    if RadioButton1.Checked then  // RGB -> HSV
    begin
      // Validação RGB
      try
        r := StrToFloat(Edit1.Text);
        if (r < 0) or (r > 255) then raise Exception.Create('R fora do intervalo');
      except
        Edit1.Text := '';
        ShowMessage('Valor inválido para R (0 a 255).');
        isValid := False;
      end;

      try
        g := StrToFloat(Edit2.Text);
        if (g < 0) or (g > 255) then raise Exception.Create('G fora do intervalo');
      except
        Edit2.Text := '';
        ShowMessage('Valor inválido para G (0 a 255).');
        isValid := False;
      end;

      try
        b := StrToFloat(Edit3.Text);
        if (b < 0) or (b > 255) then raise Exception.Create('B fora do intervalo');
      except
        Edit3.Text := '';
        ShowMessage('Valor inválido para B (0 a 255).');
        isValid := False;
      end;

      if isValid then
      begin
        r := r / 255.0;
        g := g / 255.0;
        b := b / 255.0;

        RgbToHsv(r, g, b, h, s, v);

        Edit4.Text := FormatFloat('0.00', h);
        Edit5.Text := FormatFloat('0.00', s * 100);
        Edit6.Text := FormatFloat('0.00', v * 100);
      end;
    end
    else  // HSV -> RGB
    begin
      // Validação HSV
      try
        h := StrToFloat(Edit1.Text);
        if (h < 0) or (h > 360) then raise Exception.Create('H fora do intervalo');
      except
        Edit1.Text := '';
        ShowMessage('Valor inválido para H (0 a 360).');
        isValid := False;
      end;

      try
        s := StrToFloat(Edit2.Text);
        if (s < 0) or (s > 100) then raise Exception.Create('S fora do intervalo');
      except
        Edit2.Text := '';
        ShowMessage('Valor inválido para S (0 a 100).');
        isValid := False;
      end;

      try
        v := StrToFloat(Edit3.Text);
        if (v < 0) or (v > 100) then raise Exception.Create('V fora do intervalo');
      except
        Edit3.Text := '';
        ShowMessage('Valor inválido para V (0 a 100).');
        isValid := False;
      end;

      if isValid then
      begin
        s := s / 100.0;
        v := v / 100.0;

        HsvToRgb(r, g, b, h, s, v);

        Edit4.Text := IntToStr(Round(r * 255));
        Edit5.Text := IntToStr(Round(g * 255));
        Edit6.Text := IntToStr(Round(b * 255));
      end;
    end;
  except
    on E: Exception do
      ShowMessage('Erro inesperado: ' + E.Message);
  end;
end;

procedure TForm2.RadioButton2Change(Sender: TObject);
begin

end;

procedure TForm2.StaticText1Click(Sender: TObject);
begin

end;

end.

