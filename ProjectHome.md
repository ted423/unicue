**UniCue** 是包含一系列的文字编码转换工具的开源项目。同时Unicue也是其中最主要的转换工具的名字。UniCue着重于将各种编码转换到Unicode。

名称来由：Uni代表Unicode，Cue代表cuesheet。

---


## 1.3版本开发中。 ##
欢迎测试并反馈意见。
### 如遇到任何奇怪编码而Unicue又无法转换的cue/txt文件，欢迎附加到附件然后扔到邮箱：unicue{at}kuyur.info （同时标题请注明【报告乱码文件】）。 ###


## 下载程序/签出源码 ##

**这个托管页面不再更新源代码，并且由于Google关闭了文件上传功能，这个托管页面也将不再更新二进制程序**

### 程序下载 ###
Unicue\_1.3 beta6<br>
<a href='http://go.kuyur.info/Unicue_1.3beta6.zip'>http://go.kuyur.info/Unicue_1.3beta6.zip</a><br>
<a href='http://kuyur.github.io/unicue/Unicue_1.3beta6.zip'>http://kuyur.github.io/unicue/Unicue_1.3beta6.zip</a><br>


<h3>源码</h3>
<a href='https://github.com/kuyur/unicue/'>Github</a>

使用git：<br>
<pre><code>git clone https://github.com/kuyur/unicue.git<br>
</code></pre>

或者直接点击链接： <a href='https://github.com/kuyur/unicue/archive/master.zip'>https://github.com/kuyur/unicue/archive/master.zip</a>

<h3>bug报告</h3>
请前往 <a href='https://github.com/kuyur/unicue/issues'>https://github.com/kuyur/unicue/issues</a>

<hr />

<h2>Unicue</h2>
<h4>Unicue 1.3更新：</h4>
<ul><li>增加拉丁文字母支持(CP1252)<br>
</li><li>一键转换<br>
</li><li>可调整界面大小<br>
</li><li>记忆最后使用的界面大小<br>
</li><li>修正Linux平台下通过Wine运行时菜单不能正常显示的问题<br>
</li><li>支持Ctrl+A快捷键</li></ul>

<h4>Unicue 1.2更新：</h4>
<ul><li>允许选择输出编码(UTF-8/UTF-8无BOM/UTF-16LE/UTF-16BE)<br>
</li><li>增强CUE文件的自动修正功能<br>
</li><li>修正Win7以上版本在某种情况下注册到右键菜单却没有生效的Bug<br>
</li><li>修正GBK映射表中欧元符号没有正确转换的Bug<br>
</li><li>使用WTL取代MFC，静态编译解决库依赖问题（传说不需要任何第三方dll）<br>
</li><li>更换新名字 (ANSI2Unicode -> Unicue)<br>
</li><li>多语言界面 （简中，繁中，英，日）<br>
<ol><li>感谢小海对繁体中文界面提出的修改建议/Thanks for 小海's suggestions about Traditional Chinese GUI<br>
</li><li>感谢lemphek对英文界面提出的修改建议/Thanks for lemphek's suggestions about English GUI<br>
</li></ol></li><li>增加西里尔字母支持(CP1251)，涵括斯拉夫语族包括俄语，塞尔维亚语，保加利亚语等一大堆乱七八糟的语言<br>
</li><li>Linux平台下通过Wine（1.4.1）能稳定运行（菜单不能正常显示）</li></ul>

<h4>ANSI2Unicode 1.1 更新：</h4>
<ul><li>移除自身的转换函数<br>
</li><li>使用c4-lib通用字符转换环境<br>
</li><li>支持用户添加自定义字符映射表并设定转换规则<br>
</li><li>修正图标</li></ul>

<h4>ANSI2Unicode 1.0.3 功能特性：</h4>
<ul><li>完整的Unicode支持，不必为文档路径中的特殊字符担心<br>
</li><li>修复Unicode补不完计划造成的丢字问题<br>
</li><li>--内建基于UAO 2.50的Big5→Unicode字符映射表<br>
</li><li>--Big5→Unicode单向转换，解决没有安装Unicode补完计划的win平台上显示Big5文档时的字符（日文假名/日文汉字/简体汉字）丢失问题<br>
</li><li>转换结果保存为utf-8编码的文本<br>
</li><li>自动检测文本编码<br>
</li><li>支持文件拖放操作及命令行打开文档<br>
</li><li>自动修正cue中的File行音频文件扩展名<br>
</li><li>自动修正cue中的File行旧式“The True Audio”标签为“WAVE”<br>
</li><li>不改变默认打开程序的txt/log/cue右键菜单关联<br>
</li><li>提取tak/flac/ape的内嵌cue，并自动修正cue中的音频文件名<br>
</li><li>转换字符串功能<br>
</li><li>转换字符串模式下，支持拖放乱码文件名进行转换</li></ul>

<h2>Chinese Converter</h2>
<h4>Chinese Converter 1.3 更新：</h4>
<ul><li>保存配置<br>
</li><li>可调整窗口大小并记忆最后使用的窗口尺寸<br>
</li><li>添加菜单<br>
</li><li>添加覆盖文件的保存按钮</li></ul>

<h4>Chinese Converter 1.2 更新：</h4>
<ul><li>支持Unicode LE/BE以及UTF-8格式的文本读取和写入<br>
</li><li>停止使用MFC，转用WTL并静态编译（传说无需任何第三方dll）<br>
</li><li>更换图标<br>
</li><li>Linux平台下通过Wine（1.4.1）能稳定运行</li></ul>

<h4>Chinese Converter 1.0 功能特性：</h4>
<ul><li>简繁繁简转换<br>
</li><li>仅支持Unicode（LE）输入和Unicode（LE）保存转换结果<br>
</li><li>采用维基简繁繁简一对一映射表，如有问题找维基<br>
</li><li>不支持词组转换</li></ul>

<h2>Unicue Traveller</h2>
<h4>1.2功能特性：</h4>
<ul><li>基于WTL，无需安装任何运行库<br>
</li><li>完整的Unicode支持<br>
</li><li>修复Unicode补不完计划造成的丢字问题<br>
</li><li>转换结果保存为utf-8编码的文本<br>
</li><li>自动检测文本编码<br>
</li><li>自动修正cue中的File行音频文件名<br>
</li><li>自动修正cue中的File行旧式“The True Audio”标签为“WAVE”<br>
</li><li>批量修改文本文件<br>
<ol><li>任意的文件夹和文件组合的右键菜单<br>
</li><li>文件夹的空白菜单<br>
</li></ol></li><li>可指定文件类型<br>
</li><li>两种保存方式<br>
<ol><li>覆盖（备份可选）<br>
</li><li>另存为（可指定命名模板）</li></ol></li></ul>

<h2>ExtractAkaiito</h2>
<ul><li>PS2游戏アカイイト游戏脚本提取