void FFTdataHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input,textarea{font-size:20px; text-align:center}\n\
textarea{font-size:14px; text-align:left}\n\
input[type=text]{font-size:64px; font-weight:bold; text-align:right}\n\
input[type=button]{font-size:30px; font-weight:bold}\n\
</style>\n\
<script>\n\
sampSize=1024; hSampSize=sampSize/2; dataSize=sampSize+hSampSize;\n\
dataFFT=[];\n\
Distance=12345.67;\n\ 
strData='';\n\
configStr='';\n\
Start=true; DistOk=false; FFTdataOk=false;\n\
ws=new WebSocket('ws://192.168.4.1:81/');\n\
ws.onmessage=function(msg){\n\
  ws.binaryType='arraybuffer';\n\
  if (msg.data instanceof ArrayBuffer){\n\
    if (msg.data.byteLength==dataSize) {\n\
      dataBuf=new Uint8Array(msg.data,0,dataSize);\n\
      dataFFT=new Int8Array(dataSize);\n\  
      for (i=0; i<dataSize; i++) dataFFT[i]=dataBuf[i];\n\ 
      FFTdataOk=true;\n\
    }//if buffSize\n\ 
    if (msg.data.byteLength==4) {\n\
      buff=new Uint8Array(msg.data).buffer;\n\
      view=new DataView(buff);\n\
      Distance=view.getFloat32(0,true);\n\
      dist.value=Distance.toFixed(2);\n\
      DistOk=true;\n\
    }//if bufSize==4\n\ 
  }//if msg.data\n\
  else {//string\n\ 
    configStr=msg.data;\n\
  }//else\n\ 
  if (Start&&DistOk&&FFTdataOk) {Start=false; ViewFFT();}\n\
}//onmessage \n\
function ViewFFT(){\n\
  strData=configStr+'\\n';\n\
  strData=strData+'D=\\t'+Distance.toFixed(2)+'\\n';\n\
  strData=strData+'i \\tFFT \\tFFT*\\ti*\\t(F+F*)/2\\n';\n\  
  for (i=0; i<hSampSize; i++) {\n\
    strData=strData+i+'\\t'+dataFFT[i]+'\\t'+dataFFT[sampSize-i-1];\n\
    strData=strData+'\\t'+(sampSize-i-1)+'\\t'+dataFFT[sampSize+i]+'\\n';\n\ 
  }//for i\n\
  strData=strData+'D=\\t'+Distance.toFixed(2)+'\\n';\n\
  fft.value=strData;\n\
}//ViewIQ\n\
function saveFile(){\n\
  fName='F'+DateToStr()+'.txt';\n\	  
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
<tr><td><b>FFT,dB</b></td><td><b>Distance, mm</b></td></tr>\n\
<tr><td rowspan=3><textarea id=fft cols=40 rows=60 value=''></textarea></td>\n\
<td valign=top><input type=text id=dist size=4 value='#####.##'></input></td></tr>\n\
<tr><td valign=center><input type=button id=view value='RefreshFFT' onClick=ViewFFT()></td></tr>\n\
<tr><td valign=bottom><input type=button id=save value='SaveToFile' onClick=saveFile()></td></tr>\n\
</body>");
}//SPIdataHTML