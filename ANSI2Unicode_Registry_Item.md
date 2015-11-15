1.0.3版起可以注册到文件右键菜单，程序会写如下注册表项：

[HKEY\_CLASSES\_ROOT\.uni]

&lt;BR&gt;


@="UniCue.UNI"

[HKEY\_CLASSES\_ROOT\UniCue.UNI]

&lt;BR&gt;


@="UniCue 文件"

[HKEY\_CLASSES\_ROOT\UniCue.UNI\shell]

&lt;BR&gt;


@="Open"

[HKEY\_CLASSES\_ROOT\UniCue.UNI\shell\Open]

&lt;BR&gt;


@="使用 UniCue 打开"

'AppPathName为程序路径，如E:\\Program Files (x86)\\UniCue\\Ansi2Unicode.exe

&lt;BR&gt;


[HKEY\_CLASSES\_ROOT\UniCue.UNI\shell\Open\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

[HKEY\_CLASSES\_ROOT\UniCue.UNI\shell\unicue]

&lt;BR&gt;


@="使用 UniCue 转换编码"

[HKEY\_CLASSES\_ROOT\UniCue.UNI\shell\unicue\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

[HKEY\_CLASSES\_ROOT\txtfile\shell\unicue]

&lt;BR&gt;


@="使用 UniCue 转换编码"

[HKEY\_CLASSES\_ROOT\txtfile\shell\unicue\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

'程序会查找cue文件的注册表信息，假定cue已经关联到foobar2000.CUE。如果cue文件类型的信息不存在，会将cue关联到UniCue.UNI

&lt;BR&gt;


[HKEY\_CLASSES\_ROOT\foobar2000.CUE\shell\unicue]

&lt;BR&gt;


@="使用 UniCue 转换编码"

[HKEY\_CLASSES\_ROOT\foobar2000.CUE\shell\unicue\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

[HKEY\_CLASSES\_ROOT\Applications\notepad.exe\shell\unicue]

&lt;BR&gt;


@="使用 UniCue 转换编码"

[HKEY\_CLASSES\_ROOT\Applications\notepad.exe\shell\unicue\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

[HKEY\_CLASSES\_ROOT\Applications\ANSI2Unicode.exe\shell\open\command]

&lt;BR&gt;


@="\"AppPathName\" \"%1\""

'AppFolder为程序目录，如E:\\Program Files (x86)\\UniCue

&lt;BR&gt;


[HKEY\_CLASSES\_ROOT\UniCue.UNI\DefaultIcon]

&lt;BR&gt;


@="AppFolder\\icons\\file.ico"

1.0.3版起可以将新建文本文档(txt)由系统默认模板变更为带utf-8 BOM标记的模板。程序会更改下面的注册表项：

&lt;BR&gt;



删除[HKEY\_CLASSES\_ROOT\.txt\ShellNew]下的 NullFile 键值

&lt;BR&gt;



写键值：（AppFolder为程序所在目录）

&lt;BR&gt;


[HKEY\_CLASSES\_ROOT\.txt\ShellNew]

&lt;BR&gt;


"FileName"="AppFolder\\null.uni"

&lt;BR&gt;



win7或vista系统在更改注册表时需要管理员权限