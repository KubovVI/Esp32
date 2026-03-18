void ctlHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input,button{font-size:80px; text-align:center}\n\
input[type=checkbox]{width:80px; height:80px;}\n\
input[type=text]{font-size:120px; font-weight:bold; text-align:right}\n\ 
.aref {font-size:40px}\n\
</style>\n\
<script>\n\
configStr='';\n\
winOn=1;\n\
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
  if (configStr.includes('win: None')) _win.checked=false;\n\ 
  else _win.checked=true;\n\
}//decodeConfig\n\
function changeWin(){\n\
  if (winOn) {ws.send('#w=0'); winOn=0;}\n\ 
  else {ws.send('#w=1'); winOn=1;}\n\
}//changeWin\n\
</script>\n\
</head><body>\n\
<table border=0 cellpadding=5 align=center>\n\
<tr><td>Distance, mm</td></tr>\n\
<tr><td><input type=text id=Dist size=4 value='#####.##'</input></td></tr>\n\
</table>\n\
<br><input type=button id=_ctl value=Config></input>\n\
<br><input type=checkbox id=_win checked onClick=changeWin()> FFT window</input>\n\
</body>");
}//ctlHTML