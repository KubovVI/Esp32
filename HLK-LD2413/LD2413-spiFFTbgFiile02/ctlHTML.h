void ctlHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input,button{font-size:60px; text-align:center}\n\
input[type=checkbox]{width:60px; height:60px;}\n\
input[type=text]{font-size:120px; font-weight:bold; text-align:right}\n\ 
.aref {font-size:40px}\n\
</style>\n\
<script>\n\
configStr='';\n\
winOn=1; bgOn=0;\n\
ws=new WebSocket('ws://192.168.4.1:81/');\n\
ws.onmessage=function(msg){\n\
  ws.binaryType='arraybuffer';\n\
  if (msg.data instanceof ArrayBuffer){\n\
    if (msg.data.byteLength==4) {\n\
      buff=new Uint8Array(msg.data).buffer;\n\
      view=new DataView(buff);\n\
      Distance=view.getFloat32(0,true);\n\
      Dist.value=Distance.toFixed(2);\n\
    }//if bufSize\n\  
  }//if msg.data\n\
  else {//string\n\ 
    configStr=msg.data+''; _ctl.value=configStr;\n\
    decodeConfig();\n\
  }//else\n\ 
}//onmessage\n\
function decodeConfig(){\n\
  if (configStr.includes('win:None')) _win.checked=false;\n\ 
  else _win.checked=true;\n\
  if (configStr.includes('delBG:N')) _BG.checked=false;\n\ 
  else _BG.checked=true;\n\
}//decodeConfig\n\
function changeWin(){\n\
  if (winOn) {ws.send('#w=0'); winOn=0;}\n\ 
  else {ws.send('#w=1'); winOn=1;}\n\
}//changeWin\n\
function changeBG(){\n\
  if (bgOn) {ws.send('#bg=0'); bgOn=0;}\n\ 
  else {ws.send('#bg=1'); bgOn=1;}\n\
}//changeBG\n\
function setBG(){\n\
  if (confirm('Do You want to overwrite BGfile?')) ws.send('##writeBGfile');\n\ 
}//setBG\n\
</script>\n\
</head><body>\n\
<table border=0 cellpadding=5 align=center>\n\
<tr><td><input type=button value=writeBGfile onClick=setBG()></td></tr>\n\
<tr><td><input type=text id=Dist size=4 value='#####.##'</input></td></tr>\n\
</table>\n\
<input type=button id=_ctl value=Config></input>\n\
<br><input type=checkbox id=_win checked onClick=changeWin()> FFT window</input>\n\
<br><input type=checkbox id=_BG checked onClick=changeBG()> remove BG</input>\n\
<br><font class=aref>Get BG data <a href=/bg>/bg</a></font>\n\
</body>");
}//ctlHTML