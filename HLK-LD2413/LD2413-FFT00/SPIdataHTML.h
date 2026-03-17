void SPIdataHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input,textarea{font-size:20px; text-align:center}\n\
textarea{font-size:14px; text-align:left}\n\
input[type=text]{font-size:64px; font-weight:bold; text-align:right}\n\
input[type=button]{font-size:30px; font-weight:bold}\n\
</style>\n\
<script>\n\
sampSize16=1024; frameSize16=(sampSize16+2)*2; blockSize16=frameSize16*2; blockSize8=blockSize16*2;\n\
dataI=[[],[]]; dataQ=[[],[]];\n\
Distance=12345.67;\n\ 
strData='';\n\
Start=true; DistOk=false; SPIdataOk=false;\n\
ws=new WebSocket('ws://192.168.4.1:81/');\n\
ws.onmessage=function(msg){\n\
  ws.binaryType='arraybuffer';\n\
  if (msg.data instanceof ArrayBuffer){\n\
    if (msg.data.byteLength==blockSize8) {\n\
      dataSPI8=new Uint8Array(msg.data,0,blockSize8);\n\
      dataSPI16=new Int16Array(blockSize16);\n\  
      view=new DataView(dataSPI8.buffer);\n\
      for (i=0; i<blockSize16; i++) dataSPI16[i]=view.getInt16(i*2,false);\n\ 
      for (f=0; f<2;f++){ offset16=f*frameSize16+2;\n\
        for (i=0; i<sampSize16; i++) { w16=offset16+i*2;\n\
          dataI[f][i]=dataSPI16[w16]; \n\
          dataQ[f][i]=dataSPI16[w16+1];\n\
        }//for i\n\
      }//for f\n\
      SPIdataOk=true;\n\
    }//if buffSize==blockSize\n\ 
    if (msg.data.byteLength==4) {\n\
      buff=new Uint8Array(msg.data).buffer;\n\
      view=new DataView(buff);\n\
      Distance=view.getFloat32(0,true);\n\
      dist.value=Distance.toFixed(2);\n\
      DistOk=true;\n\
    }//if bufSize==4\n\ 
  }//if msg.data\n\
  if (Start&&DistOk&&SPIdataOk) {Start=false; ViewIQ();}\n\
}//onmessage \n\
function ViewIQ(){\n\
  strData='D=\\t'+Distance.toFixed(2)+'\\n';\n\
  strData=strData+'* \\t 0 \\t\\t 1 \\n';\n\  
  for (i=0; i<sampSize16; i++) {\n\
    strData=strData+i+'\\t'+dataI[0][i]+'\\t'+dataQ[0][i]+'\\t';\n\ 
    strData=strData+dataI[1][i]+'\\t'+dataQ[1][i]+'\\n';\n\  
  }//for i\n\
  strData=strData+'D=\\t'+Distance.toFixed(2)+'\\n';\n\
  iq.value=strData;\n\
}//ViewIQ\n\
function saveFile(){\n\
  fName='S'+DateToStr()+'.txt';\n\	  
  tBlob=new Blob([strData],{type:'text/plain'});\n\
  aLink=document.createElement('a');\n\
  aLink.download=fName;\n\
  aLink.href=window.URL.createObjectURL(tBlob);\n\
  document.body.appendChild(aLink);\n\
  aLink.click();\n\ 
}//saveFile\n\
function DateToStr(){ var Dt=new Date();\n\
  yy=Dt.getFullYear(); mn=decToStr2(Dt.getMonth()+1); dd=decToStr2(Dt.getDate());\n\ 
  var s=''+yy+mn+dd; hh=decToStr2(Dt.getHours()); mm=decToStr2(Dt.getMinutes());\n\ 
  s+=hh+mm; return s;\n\
}//DateToStr\n\
function decToStr2(v) {return v.toString(10).padStart(2,'0')}\n\
</script>\n\
</head><body>\n\
<table border=0 cellpadding=5 align=center>\n\
<tr><td><b>0:(I Q) &nbsp 1:(I Q)</b></td><td><b>Distance, mm</b></td></tr>\n\
<tr><td rowspan=3><textarea id=iq cols=40 rows=60 value=''></textarea></td>\n\
<td valign=top><input type=text id=dist size=4 value='#####.##'></input></td></tr>\n\
<tr><td valign=center><input type=button id=view value='RefreshIQ' onClick=ViewIQ()></td></tr>\n\
<tr><td valign=bottom><input type=button id=save value='SaveToFile' onClick=saveFile()></td></tr>\n\
</body>");
}//SPIdataHTML