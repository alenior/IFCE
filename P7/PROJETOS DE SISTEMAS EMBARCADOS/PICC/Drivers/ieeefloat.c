int32 f_PICtoIEEE(float f)
{
   int16 c[2];
   
   memcpy((int8*)c  , ((int8*)&f)+3, 1);
   memcpy((int8*)c+1, ((int8*)&f)+2, 1);
   memcpy((int8*)c+2, ((int8*)&f)+1, 1);
   memcpy((int8*)c+3, ((int8*)&f)  , 1);
   c[1] = ((c[1] >> 1) & 0x7F80) + (c[1] & 0x7F) + ((c[1] & 0x80) << 8);
   return ((int32)c[1] << 16) | c[0];
}


float f_IEEEtoPIC(int32 f)
{
   float ret;
   int16 temp;
   
   memcpy(&temp, ((int8*)&f)+2, 2);
   temp = ((temp << 1) & 0xFF00) + (temp & 0xFF);
   
   if(bit_test(f, 31))      // Test the sign bit
      temp |= 0x0080;
   else
      temp &= 0xFF7F;
   
   memcpy(((int8*)&ret)+3, ((int8*)&f)     , 1);
   memcpy(((int8*)&ret)+2, ((int8*)&f)+1   , 1);
   memcpy(((int8*)&ret)+1, ((int8*)&temp)  , 1);
   memcpy(((int8*)&ret)  , ((int8*)&temp)+1, 1);
   
   return ret;
}
