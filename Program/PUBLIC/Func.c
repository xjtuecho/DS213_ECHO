/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Func.c Ver: 2.0                                        Author : bure & Kewei
*******************************************************************************/
#include "Func.h"

/*******************************************************************************

*******************************************************************************/
void Mem32Cpy(u32* p, u32* q, u32 n)
{
  while(n--) *p++ = *q++;
}
/*******************************************************************************

*******************************************************************************/
void Mem32Set(u32* p, u32 Word, u32 n)
{
  while(n--) *p++ = Word;
}
/*******************************************************************************
  16 bites 大端模式数据转换为小端模式
*******************************************************************************/
void Rev16(u16* pBuf)
{
  asm(" LDRH    R1, [R0] ");
  asm(" REV16   R1, R1 ");
  asm(" STRH    R1, [R0] ");
}
/*******************************************************************************
  32 bites 大端模式数据转换为小端模式
*******************************************************************************/
void Rev32(u32* pBuf)
{
  asm(" LDR     R1, [R0] ");
  asm(" REV     R1, R1 ");
  asm(" STR     R1, [R0] ");
}
/*******************************************************************************
  计算 x 的 y 次方
*******************************************************************************/
u32 Power(u8 x, u8 y)
{
  u32 m = x;

  if(y == 0) return 1;
  while (--y) m *= x;
  return m;
}
/*******************************************************************************
  计算 10 的 x 次方
*******************************************************************************/
u32 Exp(u8 x)
{
  u32 m = 1;

  while(x--) m *= 10;
  return m;
}
/*******************************************************************************
  在数据区中查找第 Idx 个字符串的起始地址
*******************************************************************************/
u8* SeekStr(u8* ptr, u8 Idx)
{
  while(Idx--) while(*ptr++);
  return ptr;
}
/******************************************************************************/
/*  Char2Nib:  ASCII character Change to 4 Bits HEX data                      */
/******************************************************************************/
u8 Char2Nib(u8 x)
{
  uc8 Hexcode[17]="0123456789ABCDEF";

  if((x >= 'a')&&(x <= 'z')) x -=32;     // 小写改大写
  for(u8 i = 0; i < 16; i++){
    if(Hexcode[i] == x) return i;        // 将字符转为4位十六进制数值
  }
  return 0;
}
/*******************************************************************************
 Value2Str: 32位数转e位有效数字符串 + 量纲字符串（结构为Unit[][6]）+  模式
*******************************************************************************/
void Value2Str(u8 *p, s32 n, uc8 *pUnit, s8 e, u8 Mode)
{
  s16 i = 0;
  s32 m = n, c = 5;

  if(Mode == SIGN){
    if(n == 0) *p++ = ' ';
    if(n >  0) *p++ = '+';
    if(n <  0){*p++ = '-'; n = -n;}
  }else *p++ = ' ';

  while(m >= 10){m /= 10; i++;} // 计算 n 的有效位数 i
  if((i%3 == 2)&&(e == 2)) e++;
  m = n; i = 0;
  while(m >= 10){
    m /= 10;
    if(++i > e) c *= 10;        // n 的有效位数 i 大于e则计算四舍五入值
  }
  if(i >= e) n += c;            // n 加上四舍五入值
  m = n; i = 0;
  while(m >= 10){m /= 10; i++;} // 重新计算 n 的有效位数 i

  m = i/3;                      // 计算量纲单位取值偏移量
  while(e--){
    *p++ = '0'+ n/Exp(i);
    if(e &&(i%3 == 0)) *p++ = '.';
    n = (i < 0)? 0 : n%Exp(i);
    i--;
  }
  pUnit += 6*m;                 //
  do {*p++ = *pUnit;}
  while(*pUnit++);              // 加上量纲字符字符串
}

/*******************************************************************************
 Two ASCII character Change to 1 Byte HEX data
*******************************************************************************/
u8 Str2Byte(u8 x,u8 y) // 双ASCII字符转1字节二进制数
{
  uc8 Hexcode[17]="0123456789ABCDEF";
  u8 i, Temp=0;

  if(x>='a' && x<='z')  x-=32;     // 小写改大写
  if(y>='a' && y<='z')  y-=32;     // 小写改大写
  for(i=0;i<16;i++){
    if(Hexcode[i]==x)  Temp+=i*16; // 将字符转为高4位十六进制数值
  }
  for(i=0;i<16;i++){
    if(Hexcode[i]==y)  Temp+=i;    // 将字符转为低4位十六进制数值
  }
  return Temp;
}

/*******************************************************************************
 u16ToDec4Str: 无符号16位二进制数转4位十进制字符串，有效数字前填空格
*******************************************************************************/
void u16To4DecStr(u8 *p, u16 n)
{
  if(n/10000){
    *p++ = 'O';
    *p++ = 'v';
    *p++ = 'e';
    *p++ = 'r';
    *p   = 0;
    return;
  }
  *p++ = '0'+n/1000;
  n %= 1000;
  *p++ = '0'+n/100;
  n %= 100;
  *p++ = '0'+n/10;
  n %= 10;
  *p++ = '0'+n;
  *p = 0;
  if(p[-4] == '0'){
    p[-4] = ' ';
    if(p[-3] == '0'){
      p[-3] = ' ';
      if(p[-2] == '0') p[-2] = ' ';
    }
  }
}
/*******************************************************************************
 u16ToDec5Str: 无符号16位二进制数转5位十进制字符串
*******************************************************************************/
void u16To5DecStr(u8 *p, u16 n)
{
  *p++ = '0'+n/10000;
  n %= 10000;
  *p++ = '0'+n/1000;
  n %= 1000;
  *p++ = '0'+n/100;
  n %= 100;
  *p++ = '0'+n/10;
  n %= 10;
  *p++ = '0'+n;
  *p = 0;
}
void s16To5DecStr(u8 *p, s16 n)
{
  if(n >= 0) *p++ = '+';
  else      {*p++ = '-'; n = -n;}
  u16To5DecStr(p, n);
}
/*******************************************************************************
 u8ToDec3: Change Byte to 3 decimal number string
*******************************************************************************/
void u8To3DecStr(u8 *p, u8 n)
{
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 s8ToPercen: Change sign char to +(-)x.xx string
*******************************************************************************/
void s8ToPercen(u8 *p, s8 n)
{
    if(n >= 0)  *p++ = '+';
    else {
      *p++ = '-';
      n = -n;
    }
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '.';
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 s8ToDec2: Change sign char to 2 decimal number string
*******************************************************************************/
void s8To2DecStr(u8 *p, s8 n)
{
    if(n >= 0)  *p++ = '+';
    else {
      *p++ = '-';
      n = -n;
    }
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 u8ToDec2: Change Byte to 2 decimal number string
*******************************************************************************/
void u8To2DecStr(u8 *p, u8 n)
{
//    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Char2Hex: Change Byte to 2 hex number string
*******************************************************************************/
void u8To2HexStr(u8 *p, u8 n)
{
    if(n/16>9) *p++ = 'A'+(n/16-10);
    else       *p++ = '0'+n/16;
    n %= 16;
    if(n>9)    *p++ = 'A'+(n-10);
    else       *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Half2Hex: Change 2Bytes to 4 hex number string
*******************************************************************************/
void u16To4HexStr(u8 *p, u16 n)
{
    if(n/0x1000 >9) *p++ = 'A'+(n/0x1000-10);
    else            *p++ = '0'+ n/0x1000;
    n %= 0x1000;
    if(n/0x100 >9)  *p++ = 'A'+(n/0x100-10);
    else            *p++ = '0'+ n/0x100;
    n %= 0x100;
    if(n/0x10 >9)   *p++ = 'A'+(n/0x10-10);
    else            *p++ = '0'+ n/0x10;
    n %= 0x10;
    if(n >9)        *p++ = 'A'+(n-10);
    else            *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Word2Hex: Change 4 Bytes to 8 hex number string
*******************************************************************************/
void u32To8HexStr(u8 *p, u32 n)
{
  s8 i, k;

  for(i=28; i>=0; i-=4){
    k = ((n >> i)& 0xF);
    if(k > 9) *p++ = 'A'+ k-10;
    else      *p++ = '0'+ k;
  }
  *p = 0;

}
/*******************************************************************************
 Int_sqrt: unsigned int square root
*******************************************************************************/
u16 Sqrt32(u32 n)
{ u32 k;
  if ( n == 0 ) return 0;
  k = 2*Sqrt32(n/4)+1;
  if ( k*k > n ) return k-1;
  else return k;
}
/*******************************************************************************

*******************************************************************************/
void dectostr(u8 *nStr,u8* decpos, u32 Num)
{
#define GCC

  int m=1,n=10,unit=1,i;
  u8 str[6],str1[6];
  int ynum;
#ifdef GCC
  int tnum;
  I32STR_RES r[1];
#endif
  if(Num==0){
    *decpos=4;
    for(i=0;i<5;i++)nStr[i]='-';
    return;}
  ynum=Num;
  while(1){
    if((Num/n)==0)break;
    else{
      m++; n*=10;
      if(m>3){m=1; unit++;Num/=1000;n=10;}
    }
  };
#ifndef GCC
    sprintf((char*)str,"%d",Num);
#else
    Int32String(r,Num,m);
    for(i=0;i<m;i++)str[i]=r->str[i];
#endif
    if(ynum>1000){
      if(unit==2){
#ifndef GCC
        sprintf((char*)str1,"%3d",ynum%1000);
#else
        tnum=ynum%1000;
        Int32String(r,tnum,3);
        for(i=0;i<3;i++)str1[i]=r->str[i];
#endif
      }
      else{
#ifndef GCC
        sprintf((char*)str1,"%3d",ynum%1000000);
#else
        tnum=ynum%1000000;
        Int32String(r,tnum,3);
        for(i=0;i<3;i++)str1[i]=r->str[i];
#endif
      }
    }
    for(i=0;i<9;i++)nStr[i]=0;
    for(i=0;i<m;i++)nStr[i]=str[i];
    nStr[m++]='.';
    for(i=0;i<(5-m);i++)nStr[m+i]=0x30;
    if(ynum>1000)
    { for(i=0;i<=3;i++)
      {
        if((str1[i]>=0x30)&& (str1[i]<0x3a))nStr[m]=str1[i];
        m++;
      }
    }
     nStr[5]=0;
    *decpos=unit;
 }
/*******************************************************************************
 Int32String_sign:带符号32位数转3位有效数字字符串
*******************************************************************************/
void Int32String_sign(I32STR_RES *r, s32 n)
{
  u32 i, m, c, e=3, fixlen;
  u8 *p = r->str;

  fixlen = e+2;
  if(n == 0x80000000){
    *p++ = ' ';
    *p++ = '.';
    *p++ = ' ';
    *p++ = ' ';
    *p++ = ' ';
    *p = 0;
    r->decPos = 4;
    r->len = p-r->str;
    return;
  }
  if ( n == 0 ) {
    *p++ = '+';
    *p++ = '0';
    *p++ = '.';
    *p++ = '0';
    *p++ = '0';
    *p = 0;
    r->decPos = 0;
    r->len = p-r->str;
    return;
  }
  if ( n > 0 ) *p++ = '+';
  else {
    *p++ = '-';
    n = -n;
  }
  m = n;
  i = 0;
  c = 5;
  while ( m >= 10 ) {
    m /= 10;
    if ( ++i > e ) c *= 10;
  }
  if ( i >= e ) n += c;
  m = n;
  i = 0;
  while ( m >= 10 ) {
    m /= 10;
    i++;
  }
  r->decPos = i/3;
  switch ( i ) {
  case 9:
    *p++ = '0'+n/1000000000;
    if ( --e == 0 ) break;
    n %= 1000000000;
    *p++ = '.', i = 0;
  case 8:
    *p++ = '0'+n/100000000;
    if ( --e == 0 ) break;
    n %= 100000000;
  case 7:
    *p++ = '0'+n/10000000;
    if ( --e == 0 ) break;
    n %= 10000000;
  case 6:
    *p++ = '0'+n/1000000;
    if ( --e == 0 ) break;
    n %= 1000000;
    if ( i ) *p++ = '.', i = 0;
  case 5:
    *p++ = '0'+n/100000;
    if ( --e == 0 ) break;
    n %= 100000;
  case 4:
    *p++ = '0'+n/10000;
    if ( --e == 0 ) break;
    n %= 10000;
  case 3:
    *p++ = '0'+n/1000;
    if ( --e == 0 ) break;
    n %= 1000;
    if ( i ) *p++ = '.', i = 0;
  case 2:
    *p++ = '0'+n/100;
    if ( --e == 0 ) break;
    n %= 100;
  case 1:
    *p++ = '0'+n/10;
    if ( --e == 0 ) break;
    n %= 10;
  case 0:
    *p++ = '0'+n;
  }
  while ( p < r->str+fixlen )  *p++ = ' ';
  *p = 0;
  r->len = p-r->str;
}
/*******************************************************************************
 Int32String:无符号32位数转e位有效数字字符串
*******************************************************************************/
void Int32String(I32STR_RES *r, u32 n, u32 e)
{
  u32 i, m, c, fixlen;
  u8 *p = r->str;

//  fixlen = e+2;
  fixlen = e+1;
  if(n == 0x80000000){
    *p++ = ' ';
    *p++ = '.';
    *p++ = ' ';
    *p++ = ' ';
    *p++ = ' ';
    *p = 0;
    r->decPos = 4;
    r->len = p-r->str;
    return;
  }
  if ( n == 0 ) {
    *p++ = '0';
    *p++ = '.';
    *p++ = '0';
    *p++ = '0';
    *p++ = '0';
    *p = 0;
    r->decPos = 0;
    r->len = p-r->str;
    return;
  }
  m = n;
  i = 0;
  c = 5;
  while ( m >= 10 ) {
    m /= 10;
    if ( ++i > e ) c *= 10;
  }
  if ( i >= e ) n += c;
  m = n;
  i = 0;
  while ( m >= 10 ) {
    m /= 10;
    i++;
  }
  r->decPos = i/3;
  switch ( i ) {
  case 9:
    *p++ = '0'+n/1000000000;
    if ( --e == 0 ) break;
    n %= 1000000000;
    *p++ = '.', i = 0;
  case 8:
    *p++ = '0'+n/100000000;
    if ( --e == 0 ) break;
    n %= 100000000;
  case 7:
    *p++ = '0'+n/10000000;
    if ( --e == 0 ) break;
    n %= 10000000;
  case 6:
    *p++ = '0'+n/1000000;
    if ( --e == 0 ) break;
    n %= 1000000;
    if ( i ) *p++ = '.', i = 0;
  case 5:
    *p++ = '0'+n/100000;
    if ( --e == 0 ) break;
    n %= 100000;
  case 4:
    *p++ = '0'+n/10000;
    if ( --e == 0 ) break;
    n %= 10000;
  case 3:
    *p++ = '0'+n/1000;
    if ( --e == 0 ) break;
    n %= 1000;
    if ( i ) *p++ = '.', i = 0;
  case 2:
    *p++ = '0'+n/100;
    if ( --e == 0 ) break;
    n %= 100;
  case 1:
    *p++ = '0'+n/10;
    if ( --e == 0 ) break;
    n %= 10;
  case 0:
    *p++ = '0'+n;
  }
  while ( p < r->str+fixlen )  *p++ = ' ';
  *p = 0;
  r->len = p-r->str;
}


/********************************* END OF FILE ********************************/
