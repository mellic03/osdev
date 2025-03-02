

global setGdt

gdtr DW 0 ; For limit storage
     DQ 0 ; For base storage

setGdt:
   MOV  [gdtr],   DI
   MOV  [gdtr+2], RSI
   LGDT [gdtr]
   RET