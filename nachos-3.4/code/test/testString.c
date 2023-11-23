#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 255
int main()
{

	const char* asciiTable =
    "Dec  Char   Dec  Char   Dec  Char   Dec  Char\n"
    "---- -----  ---- -----  ---- -----  ---- -----\n"
    "  0  (NUL)   32  (SP)    64  @       96  `\n"
    "  1  (SOH)   33  !       65  A       97  a\n"
    "  2  (STX)   34  \"       66  B       98  b\n"
    "  3  (ETX)   35  #       67  C       99  c\n"
    "  4  (EOT)   36  $       68  D      100  d\n"
    "  5  (ENQ)   37  %       69  E      101  e\n"
    "  6  (ACK)   38  &       70  F      102  f\n"
    "  7  (BEL)   39  '       71  G      103  g\n"
    "  8  (BS)    40  (       72  H      104  h\n"
    "  9  (TAB)   41  )       73  I      105  i\n"
    " 10  (LF)    42  *       74  J      106  j\n"
    " 11  (VT)    43  +       75  K      107  k\n"
    " 12  (FF)    44  ,       76  L      108  l\n"
    " 13  (CR)    45  -       77  M      109  m\n"
    " 14  (SO)    46  .       78  N      110  n\n"
    " 15  (SI)    47  /       79  O      111  o\n"
    " 16  (DLE)   48  0       80  P      112  p\n"
    " 17  (DC1)   49  1       81  Q      113  q\n"
    " 18  (DC2)   50  2       82  R      114  r\n"
    " 19  (DC3)   51  3       83  S      115  s\n"
    " 20  (DC4)   52  4       84  T      116  t\n"
    " 21  (NAK)   53  5       85  U      117  u\n"
    " 22  (SYN)   54  6       86  V      118  v\n"
    " 23  (ETB)   55  7       87  W      119  w\n"
    " 24  (CAN)   56  8       88  X      120  x\n"
    " 25  (EM)    57  9       89  Y      121  y\n"
    " 26  (SUB)   58  :       90  Z      122  z\n"
    " 27  (ESC)   59  ;       91  [      123  {\n"
    " 28  (FS)    60  <       92  \\      124  |\n"
    " 29  (GS)    61  =       93  ]      125  }\n"
    " 30  (RS)    62  >       94  ^      126  ~\n"
    " 31  (US)    63  ?       95  _      127  (DEL)\n";

	int len; 
	PrintString("\n\t\t\t-----XUAT BANG ASCII TRONG NACHOS-----\n\n");
	PrintString(asciiTable);
	Halt();
	return 0;
}
