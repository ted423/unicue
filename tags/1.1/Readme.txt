﻿UniCue是一个转换编码的小工具，目标实现Shift-JIS、GBK、BIG5编码的文本到Unicode或者utf-8编码的快捷转换。
Uni代表Unicode，Cue代表音乐索引文件。

本项目为开源项目，采用GPL v3协议发布。http://code.google.com/p/unicue 
kuyur (kuyur@kuyur.info) http://kuyur.info/blog


码表说明：

Big5 to Unicode
采用Unicode-at-on (UAO) 2.50版本，http://moztw.org/docs/big5/table/uao250-b2u.txt
码表空间为0x00～0x7F以及0x8140～0xFEFE，字符在文本以及内存中以先高位后低位方式存放
映射表物理文件b2u-little-endian.map为按Big5顺序存放的Unicode字符，范围为0x8140～0xFFFF，不含ASCII部分
和Windows平台CP950的差异：收录了简体汉字以及日文假名

GBK to Unicode
采用Mozilla的版本，http://mxr.mozilla.org/firefox/source/intl/uconv/ucvcn/cp936map.h
码表空间为0x00～0x7F以及0x8140～0xFEFE，字符在文本以及内存中以先高位后低位方式存放
映射表物理文件gb2u-little-endian.map为按GBK顺序存放的Unicode字符，范围为0x8140～0xFFFF，不含ASCII部分
和Windows平台CP936的差异：基本一致，但多了94个字符
Unicode组织提供的CP936映射表：http://unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP936.TXT

Shift-JIS to Unicode
采用微软的CP932版本，http://unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP932.TXT
字符包括两部分：JIS X 0201:1997和JIS X 0208:1997，第一部分是ASCII码、8bit半角片假名，第二部分是全角平/片假名、汉字
（JIS X 0208到Shift-JIS有一对一的映射关系：http://zh.wikipedia.org/zh/Shift_JIS）
Shift-JIS的码表空间是：0x00～0x7F（ASCII）、0xA1～0xDF（半角片假名）、0x8140～0x9FFC、0xE040～0xFC4B
注意0x5C和0x7E是映射到Unicode的0x005C和0x007E，全角字符高位需要避开0xA1～0xDF
和Windows平台CP932的差异：一致。CP932比JIS X 208:1997多收录388个字符
映射表物理文件jis2u-little-endian.map由不连续的两部分组成，0xA1～0xDF、0x8140～0xFFFF（补齐），因此有两个偏移

EUC-KR to Unicode
采用微软CP949版本，http://unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP949.TXT
码表空间为0x00～0x7F以及0x8140～0xFEFE，字符在文本以及内存中以先高位后低位方式存放
映射表物理文件kr2u-little-endian.map为按EUC-KR顺序存放的Unicode字符，范围为0x8140～0xFFFF，不含ASCII部分
和Windows平台CP949的差异：一致
