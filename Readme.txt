UniCue是一个转换编码的小工具，目标实现Shift-JIS、GBK、BIG5编码的文本到Unicode或者utf-8编码的快捷转换。
Uni代表Unicode，Cue代表音乐索引文件。

本项目为开源项目，采用GPL v3协议发布。
kuyur (kuyur@kuyur.info)


码表说明：

Big5 to Unicode
采用Unicode-at-on (UAO) 2.50版本，http://moztw.org/docs/big5/table/uao250-b2u.txt
码表空间为0x00～0x7F以及0x8140～0xFEFE，字符在文本以及内存中以先高位后低位方式存放
映射表物理文件b2u-little-endian.map为按Big5顺序存放的Unicode字符，范围为0x8140～0xFEFE，不含ASCII部分
和Windows平台CP950的差异：收录了简体汉字以及日文假名

GBK to Unicode
采用Mozilla的版本，http://mxr.mozilla.org/firefox/source/intl/uconv/ucvcn/cp936map.h
码表空间为0x00～0x7F以及0x8140～0xFEFE，字符在文本以及内存中以先高位后低位方式存放
映射表物理文件gb2u-little-endian.map为按GBK顺序存放的Unicode字符，范围为0x8140～0xFEFE，不含ASCII部分
和Windows平台CP936的差异：基本一致，但多了94个字符
Unicode组织提供的CP936映射表：http://unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP936.TXT