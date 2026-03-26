void DistanceHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,button{font-size:80px; text-align:center}\n\
input[type=text]{font-size:120px; font-weight:bold; text-align:right}\n\ 
.aref {font-size:40px}\n\
</style>\n\
<script>\n\
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
}//onmessage \n\
</script>\n\
</head><body>\n\
<table border=0 cellpadding=5 align=center>\n\
<tr><td>Distance, mm</td></tr>\n\
<tr><td><input type=text id=Dist size=4 value='#####.##'</input></td></tr>\n\
</table>\n\
<font class=aref>\n\
SPI raw data:\n\
<br>Table <a href=/spi>/spi</a>; Graph <a href=/raw>/raw</a>\n\
<br><br>FFT data:\n\ 
<br>Table <a href=/fft>/fft</a>; Graph <a href=/gr>/gr</a>; Map <a href=/map>/map</a>\n\
<br><br>Goto Control <a href=/ctl>/ctl</a>\n\
</font>\n\
</body>");
}//DistanceHTML