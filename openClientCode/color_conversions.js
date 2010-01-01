/*
  Color Converter
  Copyright 2005 David McCombs
  Attribution: 
  
  formulas taken from http://www.easyrgb.com/math.php
  Thanks EasyRGB!
  
  also for HSV taken from  http://www.salleurl.edu/~manuellv/Public/english.4.1.html
*/

function colorConverter()
{
  // RGB VALUES
  this.R = new Number(0); 
  this.G = new Number(0);
  this.B = new Number(0);
  
  // CMYK VALUES
  this.C = new Number(0); 
  this.M = new Number(0);
  this.Y = new Number(0);
  this.K  = new Number(0);
  
  // HSV values
  this.H = new Number(0); 
  this.S = new Number(0);
  this.V = new Number(0);
  
  // Method fixup  
  this.CMYtoRGB = colorConverter_CMYtoRGB;
  this.RGBtoCMY = colorConverter_RGBtoCMY;
  this.CMYtoCMYK = colorConverter_CMYtoCMYK;
  this.CMYKtoCMY = colorConverter_CMYKtoCMY;
  this.CMYKtoRGB = colorConverter_CMYKtoRGB;
  this.RGBtoCMYK = colorConverter_RGBtoCMYK;
  this.RGBtoHSV = colorConverter_RGBtoHSV;
  this.HSVtoRGB = colorConverter_HSVtoRGB;
}

function colorConverter_CMYtoRGB()
{
  // C = From 0 to 1, M = From 0 to 1, Y = From 0 to 1  
  this.R = round(( 1.0 - this.C ) * 255.0);
  this.G = round(( 1.0 - this.M ) * 255.0);
  this.B = round(( 1.0 - this.Y ) * 255.0);
}

function colorConverter_RGBtoCMY()
{
  // R = From 0 to 255, G = From 0 to 255, B = From 0 to 255
  this.C = 1.0 - ( this.R / 255.0 );
  this.M = 1.0 - ( this.G / 255.0 );
  this.Y = 1.0 - ( this.B / 255.0 );
}

function colorConverter_CMYtoCMYK() 
{
  //C = From 0 to 1, M = From 0 to 1, Y = From 0 to 1
  this.K = 1.0
  if ( this.C < this.K )   this.K = this.C;
  if ( this.M < this.K )   this.K = this.M;
  if ( this.Y < this.K )   this.K = this.Y;
  
  this.C = ( this.C - this.K ) / (1.0 - this.K );
  this.M = ( this.M - this.K ) / (1.0 - this.K );
  this.Y = ( this.Y - this.K ) / (1.0 - this.K );
}

function colorConverter_CMYKtoCMY()
{
  //C = From 0 to 1, M = From 0 to 1, Y = From 0 to 1, K = From 0 to 1  
  this.C = ( this.C * ( 1 - this.K ) + this.K )
  this.M = ( this.M * ( 1 - this.K ) + this.K )
  this.Y = ( this.Y * ( 1 - this.K ) + this.K )
}
function colorConverter_CMYKtoRGB()
{
  this.CMYKtoCMY();
  this.CMYtoRGB();
}
function colorConverter_RGBtoCMYK()
{
  this.RGBtoCMY();
  this.CMYtoCMYK();
}

/* 
  I am not going to re-invent the wheel.
  RGB to HSV - converted 'C' code 
  from : http://www.salleurl.edu/~manuellv/Public/english.4.1.html
  (DKMc 2005)
*/  
function colorConverter_RGBtoHSV()
{  
  this.V = this.H = this.S = 0; 
  var R=this.R;
  var B=this.B;
  var G=this.G;
  max = R;
  min = R;
  max = G > max? G: max;
  min = G < min? G: min;
  max = B > max? B: max;
  min = B < min? B: min;
  
  // set the Hue by weighting the quadrants
  var diff = max-min;
  if( diff > 0 )
  {  
     
    if( R == max )
      this.H = 60*( (G-B)/diff );
    else if( G == max )
      this.H = 60*(2+ (B-R)/diff );
    else if( B == max )  
      this.H = 60*(4+ (R-G)/diff ); 
           
  }
  if( max > 0 )
    this.S = Math.round((diff/max)*255);
  else
    this.S = 0;
  this.H = Math.round((this.H+360) % 360);    
  this.V = Math.round(max);

}
function colorConverter_HSVtoRGB()
{   
    // re-normalize the angle
    this.H = Math.round((this.H+360) % 360);      
    // reasonable defaults;
    this.R = this.G = this.B = 0;
    // normalized locals for this method
    var S = this.S/255;
    var V = this.V/255;
    if( this.S > 0 )
    {
      var Hi = Math.round(this.H/60); // truncated
      var f = (this.H/60)-Hi;         // remainder
      var p = V*(1-S);
      var q = V*(1-(S*f));
      var t = V*(1-(S*(1-f)));
      if( Hi==0 )
      {
        this.R = V;
        this.G = t;
        this.B = p;        
      }
      if( Hi==1 )
      {
        this.R = q;
        this.G = V;
        this.B = p;        
      }
      if( Hi==2 )
      {
        this.R = p;
        this.G = V;
        this.B = t;        
      }
      if( Hi==3 )
      {
        this.R = p;
        this.G = q;
        this.B = V;        
      }
      if( Hi==4 )
      {
        this.R = t;
        this.G = p;
        this.B = V;        
      }
      if( Hi==5 )
      {
        this.R = V;
        this.G = p;
        this.B = q;        
      }
    }
    else
    {
      this.R = this.G = this.B = V;
    }
    // normalize to 255 max...
    this.R = Math.round(this.R*255);
    this.G = Math.round(this.G*255);
    this.B =Math.round(this.B*255);
}

