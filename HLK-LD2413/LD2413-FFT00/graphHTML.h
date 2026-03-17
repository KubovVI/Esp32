void graphHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input{font-size:30px; text-align:center}\n\
input[type=text]{font-size:30px; font-weight:bold}\n\
input[type=button]{font-size:30px; font-weight:bold}\n\
input[type=checkbox]{width:30px; height:30px;}\n\
.menu {position:absolute;left:100px; top:100px; visibility:hidden;\n\ 
  border-style:solid; border-color:#00ff00; width:500px; height:340px;\n\ 
  background-color:#ffffffff}\n\
</style>\n\
<script>\n\
Run=true;\n\
//ws\n\
sampSize=1024; hSampSize=sampSize/2; dataSize=sampSize+hSampSize;\n\
dataFFT=[];\n\
Distance=12345.67;\n\ 
strData='';\n\
Rem='';\n\
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
      if (Run) Draw();\n\
    }//if buffSize\n\ 
    if (msg.data.byteLength==4) {\n\
      buff=new Uint8Array(msg.data).buffer;\n\
      view=new DataView(buff);\n\
      Distance=view.getFloat32(0,true);\n\
      if (Run) dist.value=Distance.toFixed(2);\n\
    }//if bufSize==4\n\ 
  }//if msg.data\n\
  else {//string\n\ 
    configStr=msg.data;\n\
  }//else\n\ 
}//onmessage \n\
function Draw(){ gr.save();\n\
  gr.fillStyle='#ffffff';\n\
  gr.fillRect(0,0,picW,picH);\n\
  if (Dmax>256) {dD=20; dDs=100;}\n\
  else {dD=10; dDs=50;}\n\
  Grid(); Graph();\n\ 
  gr.restore();\n\
}//Draw\n\
function GridL(){\n\
  gr.strokeStyle='#8080ff'; gr.lineWidth=2; gr.strokeRect(1,1,picW-2,picH-2);\n\
  gr.lineWidth=1;\n\
  for (d=0; d<Dmax; d+=dD) { x=remap(d,Dmin,Dmax,Xmin,Xmax);\n\
    if (d%dDs==0) w=3; else w=1;\n\
    gr.strokeRect(x,0,w,picH);}\n\  
  for (a=Amin; a<=Amax; a+=dA) { y=remap(a,Amin,Amax,Ymax,Ymin);\n\
    if (a%dAs==0) w=3; else w=1;\n\
    gr.strokeRect(0,y,picW,w); };\n\  
}//GridL\n\
function GridP(){dXgr=10; dYgr=50;\n\
  gr.strokeStyle='#8080ff'; gr.lineWidth=2; gr.strokeRect(1,1,picW-2,picH-2);\n\
  gr.lineWidth=1;\n\
  for (d=0; d<Dmax; d+=dD) { y=remap(d,Dmin,Dmax,Ymin,Ymax);\n\
    if (d%dDs==0) w=3; else w=1;\n\
    gr.strokeRect(0,y-1,picW,w);}\n\  
  for (a=Amin; a<=Amax; a+=dA) { x=remap(a,Amin,Amax,Xmin,Xmax);\n\
    if (a%dAs==0) w=3; else w=1;\n\
    gr.strokeRect(x-1,0,w,picH); };\n\  
}//GridP\n\
function GraphL(){\n\
  if (_cb1.checked){\n\
    gr.strokeStyle='#00ff00'; gr.lineWidth=1;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      y=remap(dataFFT[d],Amin,Amax,Ymax,Ymin);\n\
      x=remap(d,Dmin,Dmax,Xmin,Xmax);\n\
      gr.lineTo(x+1,y);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
  if (_cb2.checked){\n\
    gr.strokeStyle='#ff0000'; gr.lineWidth=1;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      y=remap(dataFFT[sampSize-1-d],Amin,Amax,Ymax,Ymin);\n\
      x=remap(d,Dmin,Dmax,Xmin,Xmax);\n\
      gr.lineTo(x+1,y);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
  if (_cb12.checked){\n\
    gr.strokeStyle='#000000'; gr.lineWidth=3;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      y=remap(dataFFT[sampSize+d],Amin,Amax,Ymax,Ymin);\n\
      x=remap(d,Dmin,Dmax,Xmin,Xmax);\n\
      gr.lineTo(x+1,y);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
}//GraphL\n\
function GraphP(){\n\
  if (_cb1.checked){\n\
    gr.strokeStyle='#00ff00'; gr.lineWidth=1;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      x=remap(dataFFT[d],Amin,Amax,Xmin,Xmax);\n\
      y=remap(d,Dmin,Dmax,Ymin,Ymax);\n\
      gr.lineTo(x,y+1);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
  if (_cb2.checked){\n\
    gr.strokeStyle='#ff0000'; gr.lineWidth=1;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      x=remap(dataFFT[sampSize-1-d],Amin,Amax,Xmin,Xmax);\n\
      y=remap(d,Dmin,Dmax,Ymin,Ymax);\n\
      gr.lineTo(x,y+1);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
  if (_cb12.checked){\n\
    gr.strokeStyle='#000000'; gr.lineWidth=3;\n\ 
    gr.beginPath();\n\
    for (d=0; d<Dmax; d++) {\n\
      x=remap(dataFFT[sampSize+d],Amin,Amax,Xmin,Xmax);\n\
      y=remap(d,Dmin,Dmax,Ymin,Ymax);\n\
      gr.lineTo(x,y+1);\n\
    };\n\  
    gr.stroke();\n\
  }//if\n\
}//GraphP\n\
function remap(x,xMin,xMax,yMin,yMax){\n\
  return(yMin+(yMax-yMin)*(x-xMin)/(xMax-xMin));\n\
}//remap\n\
function showSet(){\n\
  _Dmin.value=Dmin; _Dmax.value=Dmax;\n\
  _Amin.value=Amin; _Amax.value=Amax;\n\
  _Rem.value=Rem;\n\
  set.style.visibility='visible';\n\
};\n\
function restoreSet(){\n\
   Din=0; Dmax=hSampSize; Amin=0; Amax=120;\n\
   Rem='';\n\
  _Dmin.value=Dmin; _Dmax.value=Dmax;\n\
  _Amin.value=Amin; _Amax.value=Amax;\n\
  _cb12.checked=true;_cb1.checked=true;_cb2.checked=true;\n\
  _Rem.value=Rem;\n\
  set.style.visibility='hidden';\n\
};\n\
function hideSet(){\n\
  Dmin=parseInt(_Dmin.value); Dmax=parseInt(_Dmax.value);\n\
  Amin=parseInt(_Amin.value); Amax=parseInt(_Amax.value);\n\
  if(Dmin<0) {Dmin=0; _Dmin.value=Dmin;}\n\ 
  if(Dmax>hSampSize) {Dmax=hSampSize; _Dmax.value=Dmax;}\n\ 
  Rem=_Rem.value;\n\
  set.style.visibility='hidden';\n\
};\n\
function stopRun(){\n\
  if (Run) {Run=false; _stopRun.value='Run';}\n\
  else {Run=true; _stopRun.value='Stop';};\n\
};\n\
function drawComm(){\n\
  gr.fillText(Rem,commX,commY);\n\
  gr.fillText('D='+Distance.toFixed(0),commX,commY+40);\n\
  gr.fillText('FFT='+Dmin+'..'+Dmax,commX,commY+80);\n\
  gr.fillText('AdB='+Amin+'..'+Amax,commX,commY+120);\n\
  gr.fillText(configStr,commX,commY+160);\n\
}//drawComm\n\
function SavePNG(){\n\
  gr.fillStyle='#000000'; gr.font='bold 30px serif';\n\ 
  drawComm();\n\
  fName='A'+DateToStr()+'.png';\n\	  
  imageDataUrl=pic.toDataURL('image/png');\n\  
  alink=document.createElement('a');\n\
  alink.download=fName;\n\ 
  alink.href=imageDataUrl;\n\ 
  alink.click();\n\
};\n\
function DateToStr(){ var Dt=new Date();\n\
  yy=Dt.getFullYear(); mn=decToStr2(Dt.getMonth()+1); dd=decToStr2(Dt.getDate());\n\ 
  var s=''+yy+mn+dd; hh=decToStr2(Dt.getHours()); mm=decToStr2(Dt.getMinutes());\n\ 
  s+=hh+mm; return s;\n\
}//DateToStr\n\
function decToStr2(v) {return v.toString(10).padStart(2,'0')}\n\
</script>\n\
</head><body>\n\
<div aligh=center> <canvas id=pic></canvas></td></tr>\n\
</div>\n\
<div id=nav style='position:absolute;left:340px; top:0px; width:400px'>\n\
<input type=text id=dist size=4 style='font-size:60px;text-align:right' value='#####.##'><br>\n\  
<input type=button onClick=showSet() value=Set></input> &nbsp; &nbsp;\n\
<input type=button id=_stopRun onClick=stopRun() value=Stop></input> &nbsp; &nbsp;\n\
<input type=button  onClick=SavePNG() value=Save></input>\n\ 
</div>\n\
<div id=set class=menu>\n\
<table align=center cellpadding=5px><tr><td colspan=5><b>Settings</b></td></tr>\n\
<td>Dmin=</td><td><input type=text id=_Dmin size=2 value=0></td><td width=40px></td>\n\
<td>Dmax=</td><td><input type=text id=_Dmax size=2 value=512></td></tr>\n\
<td>Amin=</td><td><input type=text id=_Amin size=2 value=0></td><td></td>\n\
<td>Amax=</td><td><input type=text id=_Amax size=2 value=110></td></tr>\n\
<tr><td>Graph:</td><td colspan=4><input type=checkbox id=_cb12 checked>AA*;</input>&nbsp;&nbsp;\n\
<input type=checkbox id=_cb1 checked><font color=green>A</font>;</input>&nbsp;&nbsp;&nbsp;\n\
<input type=checkbox id=_cb2 checked><font color=red>A*</font>.</input></td></tr>\n\
<tr><td colspan=5>Rem:<input type=text id=_Rem size=6</td></tr>\n\
<tr><td></td><td></td><td></td><td></td><td></td></tr>\n\
<td colspan=2><input type=button onclick=restoreSet() value=Restore></td><td></td>\n\
<td colspan=2><input type=button onclick=hideSet() value=Store></td></tr>\n\
</table>\n\
</div>\n\
<script>\n\
winH=window.innerHeight; winW=window.innerWidth;\n\
mobile=true; if (winH<winW) mobile=false;\n\
if(mobile) {picH=winH-100; picW=winW-40;\n\
  Grid=GridP; Graph=GraphP;\n\
  nav.style='position:absolute;top:'+(picH-120)+';left:'+(picW-320);\n\
  commX=picW-200; commY=picH-200;\n\
} else {picH=winH-20; picW=winW-40;\n\
  Grid=GridL; Graph=GraphL;\n\
  nav.style='position:absolute; top:20px; left:'+(picW-320);\n\
  commX=picW-200; commY=25;\n\
}\n\
pic.height=picH; pic.width=picW;\n\
gr=pic.getContext('2d');\n\
Ymin=0; Ymax=picH; Xmin=0; Xmax=picW; //pixel\n\
Dmin=0; Dmax=512; dD=20; dDs=100;//Discrete FFT\n\
Amin=0; Amax=120; dA=10; dAs=50; //A,db FFT\n\
Draw();\n\
</script>\n\
</body>");
}//graphHTML