//--------------------------------------------------------------------
// CodeHQ 3.1.0 Javascript Bundle
// copyright 2000-2012, cplusplus.com - All rights reserved
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// CONFIG:
//   site-wide config
//--------------------------------------------------------------------
var CFG={	// used in Session and LoginBox
	"ajaxlogin":"/login.do",// url
	"ckssn":"ssn31",		// cookie_ssn
	"ckvis":"vis31",		// cookie_visited
	"ckal":"rel31",			// cookie_autologin
	"ssnttl":14400,			// cookiettl_ssnrefresh
	"ssnmax":84600,			// cookiettl_ssn
	"cklbl":"atp32"			// cookie for SelectedLabel()
};
var AUTOCLASSES = {	// for each DOM element of the class: "new ClassName(element)" is automatically called:
		'C_LoginBox':"LoginBox",
		'C_SwitchCases':"SwitchCases",
		'C_Collapsable':"Collapsable",
		'C_BoxLabels':"BoxLabels",	// has to go before BoxSort
		'C_BoxSort':'BoxSort'
};

function uniqueName(suggestion) {
	if (suggestion) {
		suggestion = suggestion.replace(/\W/g, '');
		if (typeof(window[suggestion]) == 'undefined') return suggestion;
		var cx = 1;
		while (typeof(window[suggestion+cx]) != 'undefined') ++cx;
		return suggestion+cx;
	}
	suggestion='u';
	if (typeof(UNIQUE_CX) == 'undefined') UNIQUE_CX=1; else ++UNIQUE_CX;
	while (typeof(window[suggestion+UNIQUE_CX]) != 'undefined') ++UNIQUE_CX; 
	return suggestion+UNIQUE_CX;
}

//--------------------------------------------------------------------
// READY: (MAIN FUNCTION)
//   to be called by HTML when doc is ready (not auto-called!)
//--------------------------------------------------------------------
function ready() {
	if (!onReady.isReady) {
//		if (AUTOCLASSESX)
//			for (int i=0; i<AUTOCLASSESX.length; ++i) for (var j in AUTOCLASSESX[i]) alert(i);
		onReady.isReady=true;
		// initialize all auto-classes:
		for (var c in AUTOCLASSES) {
			var theclass = window[AUTOCLASSES[c]];
			var els = document.getElementsByClassName(c);
			for (var i=0; i<els.length; ++i) {
				new theclass(els[i]);
			}
		}
		// create the one and only session object:
		var s = new Session();
		s.setFromCookie(CFG.ckssn,CFG.ckal,CFG.ssnttl,CFG.ssnmax);
		// call onReady registered functions:
		if (typeof(onReady.register)!='undefined')
			for (var i=0;i<onReady.register.length;i++) onReady.register[i].invoke();
		// adjust minheight: // requires #I_minheight, #I_top and #I_bottom
		var minheight = document.getElementById('I_minheight');
		if ((minheight)&&(typeof(window.innerHeight)!='undefined')) minheight.style.height = ''+(window.innerHeight-document.getElementById('I_top').offsetHeight-document.getElementById('I_bottom').offsetHeight-10)+'px';
	}
}

//--------------------------------------------------------------------
// onReady (fn,obj): calls fn(session) or obj.fn(session) *once* when "ready()" is called
function onReady(fn,obj) {
	var elem = new onReady_Callback(fn,obj);
	if (onReady.isReady) elem.invoke();
	else {
		if (typeof(onReady.register)=='undefined') {onReady.register=new Array();}
		onReady.register.push(elem);
	}
	return elem; // it can be invoked with invoke() method
}
function onReady_Callback(fn,obj) { this.fn=fn; this.obj=obj; }
onReady_Callback.prototype.invoke=function() { if (this.obj) this.fn.call(this.obj); else this.fn(); }

//--------------------------------------------------------------------
// REPLACEMENTS 
//   some replacements for browsers without native support:
//--------------------------------------------------------------------
if (!this.JSON) { this.JSON={}; JSON.parse=function(t) {return eval('('+t+')');} }
if(!String.prototype.trim) { String.prototype.trim = function () { return this.replace(/^\s+|\s+$/g,''); }; }
if (typeof document.getElementsByClassName == "undefined") {
	document.getElementsByClassName = function (c) {
		var ret = [],
			els = document.getElementsByTagName('*'),
			rx = new RegExp("(^|\\s)"+c+"(\\s|$)");
		for (i = 0, j = 0; i < els.length; i++) if ( rx.test(els[i].className) ) { ret[j] = els[i]; j++; }
		return ret;
	};
};

//--------------------------------------------------------------------
// GENERAL PURPOSE FUNCTIONS:
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// bindFunction (object,method) : binds a function to an object
function bindFunction (object,method) {return function() {return method.apply(object,arguments);}}
function cloneData (o) { if ((o==null)||(typeof(o)!='object')) return o; var temp=o.constructor(); for (var k in o) if (o.hasOwnProperty(k)) temp[k]=cloneData(o[k]); return temp; }

//--------------------------------------------------------------------
// T(text) : applies locale to text (global array ST)
function T(tt) {
	if ((typeof(ST)!='undefined')&&(ST[tt])) return ST[tt];
	else return tt;
}

//--------------------------------------------------------------------
// WhenId(elid) : format elid's title as its content (local time):
function WhenId(elid) {
	var el=document.getElementById(elid);
	var mo = new Array("Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec");
	var dt = new Date(Date.parse(el.title));
	var tx=mo[dt.getMonth()]+" "+dt.getDate()+", "+(dt.getYear()+(dt.getYear()<1900?1900:0));
	tx+=" at "+((dt.getHours()%12)==0?12:dt.getHours()%12)+":"+(dt.getMinutes()<10?"0":"")+dt.getMinutes()+(dt.getHours()<12?"am":"pm");
	el.innerHTML=tx;
}

//--------------------------------------------------------------------
// WhenAll() : apply WhenId to all #CH_zTm* elements
function WhenAll() {
	var els=document.getElementsByTagName('span');
	for (var i=0; i<els.length; i++) if (els[i].id.substr(0,6)=='CH_zTm') WhenId(els[i].id);
}

//--------------------------------------------------------------------
// btn(text,title,icon,url,extra classes) - same as PHP:FormatButton
function btn (text,title,icon,url,extraclass) { // extraclass = "narrow" for 75x16 button.
	el=document.createElement('span');
	var tx=new String;
	if (icon) tx='<b class="C_ico '+icon+'"></b>'+text;
	var attribs=' class="C_btn'+(extraclass?' '+extraclass:'')+'"'+(title?' title="'+(title)+'"':'');
	if (url) tx='<a href="'+url+'"'+attribs+'>'+tx+'</a>';
	else tx='<span'+attribs+'>'+tx+'</span>';
	el.innerHTML=tx;
	return el;
}

//--------------------------------------------------------------------
// cookieGet(cookiename): get cookie
function cookieGet(name) {
	var dc = document.cookie;
	var prefix = name + '=';
	var begin = dc.indexOf('; ' + prefix);
	if (begin == -1) { begin = dc.indexOf(prefix); if (begin != 0) return null; }
	else begin += 2;
	var end = document.cookie.indexOf(';', begin); if (end == -1) end = dc.length;
	var sret=decodeURIComponent(dc.substring(begin + prefix.length, end));
	return sret;
}

//--------------------------------------------------------------------
// cookieSet(name, value): set cookie
function cookieSet(name, value) {
    document.cookie = name + "=" + encodeURIComponent(value) + "; path=/";
}

//--------------------------------------------------------------------
// xhrRequest(): returns XMLHttpRequest object or ActiveXObject
function xhrRequest () {
	var request;
	if (window.XMLHttpRequest) { request = new XMLHttpRequest();}
	else if (window.ActiveXObject) { try {request = new ActiveXObject("Msxml2.XMLHTTP.6.0");} catch (e) {try {request = new ActiveXObject("Msxml2.XMLHTTP.3.0");} catch (e) {try {request = new ActiveXObject("Microsoft.XMLHTTP");} catch (e) {} } }	}
	return request;
}

//--------------------------------------------------------------------
// xhrAsync(url,fnOk,params): Performs asynchronic XMLHttpRequest
//	- if "params" is set, it makes a POST request, otherwise a GET
function xhrAsync (url,fnOk,params)
{
	var request = xhrRequest();

	if (request) {
		request.open (params?'POST':'GET',url,true);
		request.onreadystatechange = function () {
			if (request.readyState==4) {
				if (request.status==200) {
					fnOk (request.responseText);
				}
			}
		}
		if (params) {	// POST request
			var queryarray = new Array();
			for (var i=0; i<params.length; i++) {
				if ( ((params[i].type=='checkbox')||(params[i].type=='radio')) && (!params[i].checked) ) {}
				else if (params[i]) queryarray.push(escape(params[i].name)+'='+encodeURIComponent(params[i].value));
			}
			var querystring = queryarray.join('&');
			request.setRequestHeader('Content-type','application/x-www-form-urlencoded');
			request.send(querystring);
		}
		else {			// GET request
			request.send();
		}
	}
	return request;
}

//--------------------------------------------------------------------
// domClass(el,classname): Return whether element has class
function domClass (el,classname) {
	var needle=' '+classname+' ';
	var c=' '+el.className+' ';
	return (c.indexOf(needle)!=-1); 
}

// not used:
function domClassAdd (el,classname) {
	var classes = Array();
	if (el.className) classes= el.className.split(/\s+/);
	if (classname) {
		for (var i=0; i<classes.length; ++i) if (classes[i]==classname) return classes;
		classes.push(classname);
		el.className=classes.join(' ');
	}
	return classes;
}

// not used:
function domClassRemove (el,classname) {
	var classes = Array();
	if (el.className) classes= el.className.split(/\s+/);
	if (classname) {
		for (var i=0; i<classes.length; ++i)
			if (classes[i]==classname) { classes.splice(i,1); el.className=classes.join(' '); break; }
	}
	return classes;
}

// not used:
function domChildTag (el,tagname) {	// $(">tag")
	var ret = [];
	if (el.hasChildNodes)
		for (var i=0; i<el.childNodes.length; ++i) {
			if (el.childNodes[i].nodeName==tagname) { ret.push(el.childNodes[i]); }
		}
	return ret;
}


//--------------------------------------------------------------------
// SESSION CLASS
//   creating an object of this class, checks for cookies and calls onSession.imbue() [always]
//   * only one Session object, please
//--------------------------------------------------------------------
function Session() {
	this.sessionid="";	// not used in JS
	this.userid=0;
	this.access=0;
	this.visited=0;		// not used
	this.seen=0;		// not used
	this.last=0;
	this.npms=0;
	this.uniqid="";
	this.user="";
	this.ok=false;
	this.autoArray=new Array();
}

// Session.setFromCookie: sets the current session from cookies (uses CFG array)
Session.prototype.setFromCookie = function(ckssn,ckal,seenok,visitok) {
	var str=cookieGet(ckssn);
	if ((typeof str=='string')&&(str.length>0)) {
		var usrattr = new Array();
		var i=0;
		this.sessionid = str.substring(i,i+=16);
		this.userid = parseInt(str.substring(i,i+=8),16);
		this.access = parseInt(str.substring(i,i+=8),16);
		this.visited = parseInt(str.substring(i,i+=8),16);
		this.seen = parseInt(str.substring(i,i+=8),16);
		this.last = parseInt(str.substring(i,i+=8),16);
		this.npms = parseInt(str.substring(i,i+=8),16);
		this.uniqid = str.substring(i,i+=8);

		this.user=new String();
		while (i<str.length-1) {
			if (str.substring(i,i+2)=="00") break;
			else this.user+=unescape ("%"+str.substring(i,i+=2));
		};
		this.ok = (this.user.length>0);	// TODO: check against seenok & visitok - problem with local vs server times
	}
	if (!this.ok) {	// try autologin:
		str=cookieGet(ckal);
		if ((typeof str=='string')&&(str.length>0)) {
			xhrAsync (CFG.ajaxlogin+'?w=auto', bindFunction(this,this.setFromCookieXHR));
			return;
		}
	}
	onSession.imbue(this);
}
Session.prototype.setFromCookieXHR = function (text) {
	var ret=new Object();
	try {ret=JSON.parse(text);} catch (e) {ret.success=false;}
	if (ret.success) {this.set(ret);}
}

// Session.set(data): sets the current session from array data
Session.prototype.set=function (data) {
	if (data.user)
	{
		this.sessionid=data.sessionid;
		this.userid=data.userid;
		this.access=data.access;
		this.visited=data.visited;
		this.seen=data.seen;
		this.last=data.last;
		this.uniqid=data.uniqid;
		this.user=data.user;
		this.npms=data.npms;
	}
	else {
		this.sessionid='';
		this.userid=0;
		this.access=0;
		this.visited=0;
		this.seen=0;
		this.last=0;
		this.uniqid='';
		this.user='';
		this.npms=0;
	}
	this.ok = (data.user.length>0);
	onSession.imbue(this);
}

// Session.auth(level,userid): true id level or userid are matched
Session.prototype.auth = function (level,userid) {
	if ( (typeof(userid)!="undefined") && (userid==this.userid) ) return true;
	if (typeof(level)=="string") {
		var ACCESSLEVEL = { 'admin':32768, 'member':2, 'post':64, 'report':32, 'filter':512, 'approve':1024 };
		bm=ACCESSLEVEL[level];
	}
	else bm=level;
	if ((bm)&&((this.access&bm)==bm)) return true; else return false;
}

// onSession (fn,obj): calls fn(session) or obj.fn(session) after any session change
function onSession(fn,obj) { 
	if (typeof(onSession.register)=='undefined') {onSession.register=new Array();}
	var elem=new onSession_Callback(fn,obj);
	onSession.register.push(elem);
	elem.invoke();
	return elem; // it can be invoked with invoke() method
}

//onSession.imbue: imbues new session (and calls all onSession events with the new session object):
onSession.imbue=function(session) {
	onSession.session=session;
	if (typeof(onSession.register)!='undefined')
		for (var i=0;i<onSession.register.length;i++) {onSession.register[i].invoke();}
}

function onSession_Callback(fn,obj) { this.fn=fn; this.obj=obj; }
onSession_Callback.prototype.invoke=function() {
	if (typeof(onSession.session)=='undefined') return;
	if (this.obj) {this.fn.call(this.obj,onSession.session);}
	else {this.fn(onSession.session);}
}

// -----------------------------------------------------------------------------------------------------
// *** LoginBox autoclass: ***
// -----------------------------------------------------------------------------------------------------
//function LoginBox (elid) {
function LoginBox(el) {
	this.el = el;
	// arguments are given by the titles of the first two child elements: 
	this.ajax = ((el.hasChildNodes)&&(el.childNodes[0].title=='ajax'));
	this.redirto = ((el.hasChildNodes)&&(el.childNodes.length>1)&&(el.childNodes[1].title))? el.childNodes[1].title : window.location.href;

	this.timeout = null;
	this.objName='LB'+el.id; eval(this.objName+'=this;');
	this.cb_drawBox=onSession(this.drawBox,this);
	this.elcorner=false;	// main box with user id and buttons
	this.elfold=false;  // folded auxiliary window for login (content in #I_login)
	this.elmsg=false;	// content within auxiliary window
	this.elform=false;
}
LoginBox.prototype.httpaccess='/user/access.cgi';	// non-ajax login script (ajax is CFG.ajaxlogin)
LoginBox.prototype.drawBox = function(session) {
	this.session=session;
	if (this.timeout) {clearTimeout (this.timeout);this.timeout=null;}
	if (!this.elcorner) {
		// create corner box:
		this.elcorner=document.createElement('div');
		this.elcorner.id='I_corner';
		this.el.appendChild(this.elcorner);
		var elbuttons=document.createElement('div');
		elbuttons.id='I_userbuttons';
		this.el.appendChild(elbuttons);
	}
	if (!this.elfold) {
		// create auxiliary window (this.elfold) and its children #I_loginform, #I_loginwait, #I_loginfail:
		this.elfold=document.createElement('div');
		this.elfold.id='I_fold';
		this.elfold.style.display='none';
		var st=new String('');
		st+='<div class="deco"></div>';
		st+='<form name="I_loginform" id="I_loginform" action="'+this.httpaccess+'"'+(this.ajax?' onsubmit="'+this.objName+'.login(); return false;"':'')+' method="post">';
		st+= '<div class="fields">';
		st+= '<input type="hidden" name="w" value="login"><input type="hidden" name="y" value="1"><input type="hidden" name="to" value="'+this.redirto.replace('"','')+'">';
		st+= 'user name: <input class="txt" name="l" size="8" type="text" title="Type user name"><br>';
		st+= 'password: <input class="txt" name="p" size="8" type="password" title="Type password"><br>';
		st+= '<input class="chk" name="rememberme" id="rememberme" value="1" type="checkbox"><label for="rememberme">remember me<\/label>&nbsp;';
		st+= '<input class="btn" value="sign in" type="submit"><br>';
		st+= '<div class="forgot"><a href="'+this.httpaccess+'?w=lostpass">forgot your password?</a></div>';
		st+= '<\/div>'; // class="fields"
		st+= '<div class="openid">or sign in using:<br>';
		st+= '<a class="google" href="'+this.httpaccess+'?w=login&y=1&pr=google&to='+encodeURIComponent(window.location.pathname)+'" title="Sign in using your google account"><span><\/span><\/a>';
		st+= '<a class="yahoo" href="'+this.httpaccess+'?w=login&y=1&pr=yahoo&to='+encodeURIComponent(window.location.pathname)+'" title="Sign in using your Yahoo! account"><span><\/span><\/a>';
		st+=' <\/div>';
		st+='<\/form>';
		st+='<div id="I_loginwait"></div>';
		st+='<div id="I_loginfail">';
		st+= '<div id="I_loginfailtext"></div>';
		st+= '<a class="C_bRepeat" href="javascript:'+this.objName+'.modal(1)">try again</a>';
		st+= '<a class="C_bCancel" href="javascript:'+this.objName+'.modal(0)">cancel</a>';
		st+= '<div class="forgot"><a href="'+this.httpaccess+'?w=lostpass">forgot your password?</a></div>';		
		st+='</div>';
		st+='<div class="close"><a href="javascript:'+this.objName+'.modal(0)" title="cancel log in"><span></span></a></div>';
		this.elfold.innerHTML=st;
		this.el.appendChild(this.elfold);
		// locate children elements:
		this.elform=document.getElementById('I_loginform');
		this.elwait=document.getElementById('I_loginwait');
		this.elfail=document.getElementById('I_loginfail');
		this.elfailtext=document.getElementById('I_loginfailtext');
		this.modal(0); // hide all (while still in hidden DIV)
	}
	var st= new String('');
	if (session.ok)
	{
		st+='<div id="I_cornerinner"><div id="I_userid">';
//		if ((session.npms>0)&&(session.npms!=1000000)) st+='<a href="/user/pm.cgi">'+session.npms+'<img src="/img/newmail.gif" alt="You have '+session.npms+' new PMs" title="You have '+session.npms+' new PMs"></a><br>';
//		if ((session.npms>0)&&(session.npms!=1000000)) st+='<div><a href="/user/pm.cgi" title="You have '+session.npms+' new PMs"><span class="C_mailnew"></span>'+session.npms+'</a></div>';
		if ((session.npms>0)&&(session.npms!=1000000)) st+='<div><a href="/user/pm.cgi" title="You have '+session.npms+' new PMs"><span class="C_ico mailnew"></span> '+session.npms+'</a></div>';
		st+='<div>Logged in as: <b>'+session.user+'<\/b></div>';
		st+='<\/div><\/div>';
		st+='<form name="I_logoutform" id="I_logoutform" action="'+this.httpaccess+'"'+(this.ajax?' onsubmit="'+this.objName+'.logout(); return false;"':'')+' method="post">';
		st+='<input type="hidden" name="w" value="logout"><input type="hidden" name="y" value="1"><input type="hidden" name="to" value="'+this.redirto.replace('"','')+'">';
		st+='</form>';
		document.getElementById('I_userbuttons').innerHTML='<a href="/user/">Account</a><a id="I_btnlogout" href="javascript:'+this.objName+'.logout();">logout</a>';
	}
	else {
		st+='<div id="I_cornerinner"><div id="I_userid">Not logged in</div></div>';
		document.getElementById('I_userbuttons').innerHTML='<a href="/user/access.cgi?w=signup">register</a><a id="I_btnlogin" href="javascript:'+this.objName+'.modal(1)">log in</a>';
	}
	this.elcorner.innerHTML=st;
}

// switches between the different layouts for the modal dialog box
LoginBox.prototype.modal = function(level) {
	this.elform.style.display=(level==1?'block':'none');	// 1. form
	this.elwait.style.display=(level==2?'block':'none');	// 2. wait
	this.elfail.style.display=(level==3?'block':'none');	// 3. fail
	this.elfold.style.display=(level?'block':'none');		// 0. none
	this.elcorner.style.display=(level?'none':'block');		// (superfluous - hidden anyway)
	if (level==1) this.elform.l.focus();
}

LoginBox.prototype.login = function() {
	this.elwait.innerHTML='<div class="loginmessage"><b>logging in</b><br><img src="/img/progress.gif" width="286" height="20"><br>please wait</div>';
	this.elfailtext.innerHTML='Login timeout';
	this.timeout=setTimeout(this.objName+'.modal(3)',30000);
	xhrAsync ( CFG.ajaxlogin, bindFunction(this,this.loginoutXHR), this.elform.elements );
}

LoginBox.prototype.loginoutXHR = function(jsonArray) {	// for both login and logout
	var ret=new Object();
	try {ret=JSON.parse(jsonArray);} catch (e) {ret.success=false;}	// silent behavior (auto-initiated)
	this.elform.reset();
	if (ret.success) { this.modal(0); this.session.set(ret); }
	else { this.elfailtext.innerHTML='Bad user name or password'; this.modal(3); }
}

LoginBox.prototype.logout = function() {	// unlike login, logout catches both XHR and redirect (for Firefox)
	if (!this.ajax) {document.getElementById('I_logoutform').submit();return;}
	this.elwait.innerHTML='<div class="loginmessage"><b>logging out</b><br><img src="/img/progress.gif" width="286" height="20"><br>please wait</div>';
	this.timeout=setTimeout('"'+this.objName+'.modal(0)"',30000);
	xhrAsync ( CFG.ajaxlogin, bindFunction(this,this.loginoutXHR), document.getElementById('I_logoutform').elements ); // [ {"name":"w","value":"logout"}, {"name":"y","value":"1"}, {"name":"to","value":this.redirto} ]
}

//-----------------------------------------------------------------------------------------------------
// *** SwitchCases autoclass: *** (uses same format as SelectedLabel, could use SelectedLabel instead)
//-----------------------------------------------------------------------------------------------------
function SwitchCases(el) {
	if (!el.hasChildNodes) return;

	if ( typeof SwitchCases.cx == 'undefined' ) SwitchCases.cx=0;
	++SwitchCases.cx;
	this.objName='switch'+SwitchCases.cx; eval(this.objName+'=this;');
	this.elMenu=document.createElement('ul');
	this.labels=new Array();	// not used so far
	this.nlabels=0;
	this.views=new Array();
	this.buttons=new Array();
	for (var i=0; i<el.childNodes.length; ++i) {
		if (el.childNodes[i].title) {
			this.views[this.nlabels]=el.childNodes[i];
			this.labels[this.nlabels]=el.childNodes[i].title;
			var opt=document.createElement('li');
			opt.innerHTML='<a href="javascript:'+this.objName+'.select('+(this.nlabels+1)+')">'+el.childNodes[i].title+'</a>';
			this.buttons[this.nlabels]=opt;
			this.elMenu.appendChild(opt);
			++this.nlabels;
		}
	}
	var nil=document.createElement('li');
	nil.className='hide';
	var txt='<a href="/site/versions/"><b class="C_ico question" title="Help about versions"></b></a>';
	nil.innerHTML=txt;
	this.elMenu.appendChild(nil);
	el.insertBefore(this.elMenu, el.firstChild);
	this.el=el;	// needed?
	var autoselect = new String(); autoselect+=cookieGet(CFG.cklbl); autoselect=autoselect.split('|');
	for (var j=0; j<autoselect.length; ++j) {
		for (var i=0; i<this.nlabels; ++i) {
			if (this.labels[i]==autoselect[j]) { this.select(i+1,true); return; }
		}
	}
	this.select(1,true);	// default
}

SwitchCases.prototype.select=function(which,bNoClick) {
	if (which) {this.selected=which;this.hidden=false;}
	else if (this.hidden) {which=this.selected;this.hidden=false;}
	else this.hidden=true;
	for (var i=0; i<this.nlabels; ++i) {
		this.views[i].className = (i==which-1?'on':'off');
		this.buttons[i].className = (i==which-1?'on':'off');
	}
	if (!bNoClick) {
		var autoselect = new Array();
		if (which) autoselect.push(this.labels[which-1]);
		var former_as = new String(); former_as+=cookieGet(CFG.cklbl); former_as=former_as.split('|');
		for (var j=0; j<former_as.length; ++j) {
			if (j>10) break;
			var found = false;
			for (var i=0; i<this.nlabels; ++i) if (this.labels[i]==former_as[j]) {found=true;}
			if (!found) {autoselect.push(former_as[j]);}
		}
		cookieSet (CFG.cklbl,autoselect.join('|'));
	}
}
//-----------------------------------------------------------------------------------------------------
//*** Collapsable autoclass: *** makes one UL collapsable
// if element has also class "collapsed" -> starts as collapsed
//-----------------------------------------------------------------------------------------------------
function Collapsable(el) {	// not a class: no need for new (it stores no info in "this")
	var uls = el.getElementsByTagName('UL');
	if (uls.length>0) {
		el.innerHTML='<div class="mn"></div>'+el.innerHTML;
		if (domClass(el,'collapsed')) { el.firstChild.className='pl'; uls[0].className='nop'; }
	}
	var toggle = function() {
		var mum = this.parentNode;
//		this.className = (this.className=='pl') ? 'mn' : 'pl';	// (last)plus/minus
		mum.firstChild.className = (mum.firstChild.className=='pl') ? 'mn' : 'pl';	// (last)plus/minus
		var uls = mum.getElementsByTagName('UL');
		if (mum.hasChildNodes) for (var i=0; i<mum.childNodes.length; ++i) {
			if (mum.childNodes[i].nodeName=='UL')
				// $(mum.childNodes[i]).animate({height:"toggle"},"fast");	// uses jquery animate
				mum.childNodes[i].className=(mum.childNodes[i].className=='nop')?'yep':'nop';
		}
	};
//	el.getElementsByTagName('H4')[0].onclick = toggle;
	el.firstChild.onclick = toggle;
	// create bullets:
	var ul=uls[0];
	if (ul.hasChildNodes) {
		var last=true;
		for (var j=ul.childNodes.length-1; j>=0; --j) {
			if (ul.childNodes[j].nodeName=='LI') {
				ul.childNodes[j].innerHTML='<div class="'+(last?'last':'mid')+'"></div>'+ul.childNodes[j].innerHTML;
				last=false;
			}
		}
	}

}


//-----------------------------------------------------------------------------------------------------
//*** BoxLabels autoclass: *** Inserts a DIV.label on each sub LI
// -> .C_BoxLabels LI : affects all LI's under it
//-----------------------------------------------------------------------------------------------------
function BoxLabels(el) {
	var lis = el.getElementsByTagName('LI');
	for (var i=0; i<lis.length; ++i) if (!domClass(lis[i],'C_Collapsable')) {
		var inner = document.createElement("DIV");
		var outter = document.createElement("DIV");
		outter.className = 'label';
		outter.appendChild(inner);
		lis[i].insertBefore(outter,lis[i].firstChild);
	}
}

//-----------------------------------------------------------------------------------------------------
// *** SelectedLabel: *** (uses CFG.cklbl)
// SelectLabel () -> return array with autoselects
// SelectLabel (set) -> (get) returns the first label selected in "set" (ok if set is string)
// SelectLabel (set,selected) -> (set) unsets all in set, sets selected
//-----------------------------------------------------------------------------------------------------
function SelectedLabel(set,selected) {
	var current = new String(); current+=cookieGet(CFG.cklbl);
	current = current.split('|');
	if (typeof set === 'undefined') return current;
	if (!(set instanceof Array)) {set=[set];}
	if (typeof selected == 'undefined') {
		for (var i=0; i<current.length; ++i) for (var j=0; j<set.length; ++j) if (current[i]==set[j]) {return set[j];}
		return '';
	}
	var updated = new Array();
	for (var i=0; i<current.length; ++i)
	{
		var found=0;
		for (var j=0; j<set.length; ++j) if (current[i]==set[j]) found = 1;
		if (!found) updated.push(current[i]);
	}
	if (selected) updated.splice(0,0,selected);
	cookieSet (CFG.cklbl,updated.join('|'));
}

//-----------------------------------------------------------------------------------------------------
// *** BoxSort autoclass: *** Switches between alphabetic and groups.
// -> .C_BoxSort >UL LI (UL LI) : needs to be set on direct parent of affected UL
//-----------------------------------------------------------------------------------------------------
function BoxSort(el) {
	this.el = el;
	this.lu_groups = null;
	this.lu_alpha = null;
	this.persistent = true;
	this.alpha_on = false;
	if ( typeof BoxSort.cx == 'undefined' ) BoxSort.cx=0;
	++BoxSort.cx;
	this.objName = uniqueName(el.id); eval(this.objName+'=this;');
	if (this.persistent) { if (SelectedLabel(this.objName)) {this.alpha_on=true;} }
	// find original LU:
	for (var i=0; i<el.childNodes.length; ++i) { if (el.childNodes[i].nodeName=='UL') { this.lu_groups=el.childNodes[i]; break; } } 
	// obtain LI sub-objects and their strings:
	var sub = new Object();
	var strs = new Array(); var ssort = new Array();
	var lis = this.lu_groups.getElementsByTagName('LI');
	for (var i=0; i<lis.length; ++i) {
		var uls = lis[i].getElementsByTagName('UL');
		if (uls.length==0) {
			var txt = ( lis[i].innerText ? lis[i].innerText : list[i].textContent );
			sub[txt]=lis[i];
			strs.push(txt); ssort.push(txt);
		}
	}
	// sort sub-elements
	ssort.sort(); var sorted=true;
	for (var i=0; i<strs.length; ++i) if (ssort[i]!=strs[i]) sorted=false;
	if (sorted) return;	// no need to be sortable (already sorted)
	// create alternative LU:
	var replacement = new String();
	for (var i=0; i<ssort.length; ++i) {
		replacement+='<li class="'+sub[ssort[i]].className+'">'+sub[ssort[i]].innerHTML+'</li>';
	}
	this.lu_alpha = document.createElement ("UL");
	this.lu_alpha.innerHTML = replacement;
	this.lu_alpha.style.display = 'none';
	el.appendChild (this.lu_alpha);
	// draw toggle button:
	this.div_toggle = document.createElement ("DIV");
	this.div_toggle.onclick = bindFunction(this,this.toggle);
	this.redraw();
	var h3 = el.getElementsByTagName('H3');
	if (h3.length>0) {h3[0].insertBefore (this.div_toggle,h3[0].firstChild); }
	
}

BoxSort.prototype.redraw = function() {
	this.lu_alpha.style.display = this.alpha_on?'block':'none';
	this.lu_groups.style.display = this.alpha_on?'none':'block';
	this.div_toggle.className = this.alpha_on?'toggle_off':'toggle_on';
	this.div_toggle.title = this.alpha_on?'Original sorting':'Sort alphabetically';
}

BoxSort.prototype.toggle = function() {
	this.alpha_on = this.alpha_on?false:true;
	if (this.persistent) { SelectedLabel(this.objName,(this.alpha_on?this.objName:'')); }
	this.redraw();
};

// -----------------------------------------------------------------------------------------------------
// *** "Score" object: ***
// -----------------------------------------------------------------------------------------------------
function Score(parentid,httpscore,unique) {
	this.parentid=parentid;
	this.httpscore=httpscore;
	this.unique=unique;
	// create app:
	this.el=document.createElement('div'); this.el.className='C_Score';
	this.elTop=document.createElement('div'); this.elTop.className='top'; this.el.appendChild(this.elTop);
	this.elBot=document.createElement('div'); this.elBot.className='bot'; this.el.appendChild(this.elBot);
	this.elVote=document.createElement('div'); this.elVote.className='vote'; this.el.appendChild(this.elVote);
	this.objName='score'+unique; eval(this.objName+'=this;');
	document.getElementById(this.parentid).appendChild(this.el);
	
	onSession(this.onSession,this);
}

//Score.prototype.httpscore='/score.do';
Score.prototype.alts=new Array('not rated','poor','weak','normal','great','recommended');

Score.prototype.onSession = function(session) { this.session=session; xhrAsync ( this.httpscore+'?w=get&uq='+this.unique, bindFunction(this,this.drawVoteXHR)); }

Score.prototype.drawVoteXHR = function (jsonArray) {
	try {ret=JSON.parse(jsonArray);} catch (e) {return;}		// silent behavior (auto-initiated)
	var score;
	if (ret.success) {score=ret.score;}
	if (isNaN(score)) score=0;
	var txtop,txbot;
	if (score) {
		if ((this.session.ok)&&(score!=-1)) {txtop='Change your rating:';txbot='You rated this with '+score+' star'+(score!=1?'s':'')+' ('+this.alts[score]+')';}
		else {txtop='Rate this article'; txbot='You have already rated this.'; }
	}
	else {txtop='Rate this'; txbot='Please, choose stars to award:';}
	this.elTop.innerHTML=txtop;
	this.elBot.innerHTML=txbot;

	if ( (this.session.ok) || (!score) )
	{
		var st=new String('');
		for (var i=1;i<=5; i++)
			st+='<img src="/ico/24star.png" alt="*" width="24" height="24" id="CH_Star'+this.unique+'_'+i+'" title="'+i+' star'+(i!=1?'s':'')+': '+this.alts[i]+'" onmouseover="'+this.objName+'.updateStars('+i+',true)" onmouseout="'+this.objName+'.updateStars('+score+',false)" onclick="'+this.objName+'.submitVote('+i+')">';
		this.elVote.innerHTML=st+'<br>(hover mouse and click)';
	}
	else this.elVote.innerHTML='Please, log in to change your current rating, or sign up to create a new account.';
	this.updateStars(score,false);
}

Score.prototype.submitVote = function (score) {
	this.elBot.innerHTML='Thanks for your feedback';
	this.elVote.innerHTML='<img src="/ico/progress.gif"><br>... Please wait ...';
	xhrAsync ( this.httpscore+'?w=vote&uq='+this.unique+'&q='+score, bindFunction(this,this.drawVoteXHR) );
}

Score.prototype.updateStars = function (howmany,light) {
	for (var i=1;i<=5;i++) {
		if (elStar=document.getElementById('CH_Star'+this.unique+'_'+i)) {
			if (i<=howmany) {if (light) elStar.src='/ico/24star.png'; else elStar.src='/ico/24star-gray.png';}
			else elStar.src='/ico/24star-empty.png';
		}
	}
}


//-----------------------------------------------------------------------------------------------------
//*** "DOC" MODULE 3.0 ***
//-----------------------------------------------------------------------------------------------------
// nothing here

//-----------------------------------------------------------------------------------------------------
//*** "ART" MODULE 3.0 ***
// no longer in use!
//-----------------------------------------------------------------------------------------------------
// nothing here

//-----------------------------------------------------------------------------------------------------
// *** "FOR" MODULE 3.0 ***
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//BBEdit:
//BB Edit box -- user independent (no need to init)
//* "fields" is an associative array with keys being: title, content,... = default values (or empty string)
//-----------------------------------------------------------------------------------------------------
function BBEdit (elParent,fields,action,hiddens,onOk,onCancel) {
	this.action=action;
	this.onOk=onOk;
	this.onCancel=(typeof(onCancel)!='undefined'?onCancel:onOk);

	if (!BBEdit.count) BBEdit.count=0;	BBEdit.count++;
	this.objName='editbox'+BBEdit.count; eval(this.objName+'=this;');

	var txt= new String();
	txt+='<form name="CH_fedit" action="'+action+'" method="post" onsubmit="return false;"><table>';
	txt+='<tr><td><textarea name="content" cols="80" rows="15" onfocus="'+this.objName+'.editCount()" onkeydown="'+this.objName+'.editCount()" onkeyup="'+this.objName+'.editCount()"><\/textarea><\/td>';
	txt+='<td><i>'+T('Format')+':<\/i><br><br>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'code\')"><img src="/ico/text_code.png" width="16" height="16" alt="code" title="'+T('Source code')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'output\')"><img src="/ico/text_output.png" width="16" height="16" alt="output" title="'+T('Program output')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'quote\')"><img src="/ico/text_quote.png" width="16" height="16" alt="quote" title="'+T('Quotation')+'"><\/a>';
	txt+='<br>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'b\')"><img src="/ico/text_b.png" width="16" height="16" alt="bold" title="'+T('Bold')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'i\')"><img src="/ico/text_i.png" width="16" height="16" alt="italics" title="'+T('Italics')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'u\')"><img src="/ico/text_u.png" width="16" height="16" alt="underline" title="'+T('Underline')+'"><\/a>';
	txt+='<br>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'center\')"><img src="/ico/text_center.png" width="16" height="16" alt="center" title="'+T('Center')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'s\')"><img src="/ico/text_s.png" width="16" height="16" alt="striketrhough" title="'+T('Strikethrough')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'tt\')"><img src="/ico/text_tt.png" width="16" height="16" alt="teletype" title="'+T('Teletype (fixed font)')+'"><\/a>';
	txt+='<br>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'small\')"><img src="/ico/text_small.png" width="16" height="16" alt="small" title="'+T('Small font')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'sub\')"><img src="/ico/text_sub.png" width="16" height="16" alt="subscript" title="'+T('Subscript')+'"><\/a>';
	txt+='<a href="javascript:'+this.objName+'.editTag(\'sup\')"><img src="/ico/text_sup.png" width="16" height="16" alt="superscript" title="'+T('Superscript')+'"><\/a>';
	txt+='<br>';
	txt+='<\/td>';
	txt+='<\/tr>';
	txt+='<tr><td colspan="2">Length: <input readonly type="text" name="CH_edtCount" size="4" maxlength="4" style="text-align:right; background-color:#E0E0E0" value=""> '+T('(max=8192)');
	txt+=' &nbsp; &nbsp; <span id="I_feditholder"></span>';
//	txt+=' &nbsp; &nbsp; <a href="javascript:'+this.objName+'.editPreview()" class="C_b75" title="'+T('Preview message')+'"><img src="/ico/16eye.png" width="16" height="16" alt="eye">'+T('Preview')+'<\/a>';
//	txt+=' <a href="javascript:'+this.objName+'.editSend()" class="C_b75" title="'+T('Submit')+'"><img src="/ico/16ok.png" width="16" height="16" alt="ok">'+T('Submit')+'<\/a>';
//	txt+=' <a href="javascript:'+this.objName+'.editCancel()" class="C_b75" title="'+T('Cancel')+'"><img src="/ico/16cancel.png" width="16" height="16" alt="X">'+T('Cancel')+'<\/a>';
	for (var k in hiddens) {
		txt+='<input type="hidden" name="'+k+'" value="'+hiddens[k]+'">';
	}
	txt+=' <input type="hidden" name="y" value="1">';
	txt+='<\/td><\/tr><\/table><\/form>';
	txt+='<form name="CH_prevpost" action="'+action+'" target="previewing" method="post"><input type="hidden" name="content" value=""><input type="hidden" name="w" value="preview"><\/form>';

	this.el=document.createElement('div');
	this.el.innerHTML=txt;
	elParent.appendChild(this.el);
	
	var holder = this.el.getElementsByTagName('span')[0];
	holder.appendChild(btn('Preview','Preview message','eye','javascript:'+this.objName+'.editPreview()'));
	holder.appendChild(btn('Submit','Submit','ok','javascript:'+this.objName+'.editSend()'));
	holder.appendChild(btn('Cancel','Cancel','cancel','javascript:'+this.objName+'.editCancel()'));
	
	this.elForm=this.el.firstChild;
	this.elContent=this.elForm.getElementsByTagName('textarea')[0];
	this.elCount=this.elForm.getElementsByTagName('input')[0];
	this.elPrevForm=this.el.getElementsByTagName('form')[1];
	this.elPrevContent=this.elPrevForm.getElementsByTagName('input')[0];
	this.elContent.focus();
	
	// set default for textarea:
	if (typeof(fields['content'])!='undefined') this.elContent.value=fields['content'];
}

BBEdit.prototype.editCount = function () { this.elCount.value=this.elContent.value.length; }
BBEdit.prototype.editTag = function (bbtag)
{
	var elem=this.elContent;
	if (elem.setSelectionRange)	elem.value=elem.value.substring(0,elem.selectionStart)+'['+bbtag+']'+elem.value.substring(elem.selectionStart,elem.selectionEnd)+'[/'+bbtag+']'+elem.value.substring(elem.selectionEnd,elem.value.length);
	else if (document.selection && document.selection.createRange)
	{
		elem.focus();
		var range=document.selection.createRange();
		range.text='['+bbtag+']'+range.text+'[/'+bbtag+']';
	}
	else
		elem.value += '['+bbtag+'][/'+bbtag+']';
}

BBEdit.prototype.editCancel = function () {
	this.el.parentNode.removeChild(this.el);
	this.onCancel();
}
BBEdit.prototype.editPreview = function ()
{
	this.elPrevContent.value=this.elContent.value;
	this.elPrevForm.submit();
}
BBEdit.prototype.editSend = function ()
{
	var onOk = bindFunction(this,function(text) {this.el.parentNode.removeChild(this.el); this.onOk(text);});
	xhrAsync ( this.action, onOk, this.elForm.elements );
	this.el.innerHTML='Sending. Please wait...';
}

//---------------------------------------------------------------------------
// for_ThreadList: Manages the "new" labels in ShowThreads (and ShowForums)
// * better as object (new for_ThreadList), but can also be called as function
function for_ThreadList(elid,cookiename,now,httpbookmark) {	// if httpbookmark set, bookmark XHR requested
	this.threadlistcookiename=cookiename;
	this.now=now;
	this.httpbookmark=httpbookmark;
	this.thids=new Array();
	this.thtms=new Array();
	
	this.objName='threadlist';
	eval(this.objName+'=this;');
	
	var el=document.getElementById(elid);
	if ( (el) && (el.hasChildNodes) )
		for (var i=0; i<el.childNodes.length; ++i) {
			if ( (' '+el.childNodes[i].className+' ').indexOf('C_forThread') != -1) {
				if (el.childNodes[i].hasChildNodes()) {
					var data=el.childNodes[i].firstChild.title.split(',');
					this.thids.push(data[0]);
					this.thtms.push(data[1]);
				}
			}
		}

	if (cookiename)	this.cb_drawNewLabels=onSession(this.drawNewLabels,this);
	if (httpbookmark) onSession(this.drawBookmarks,this);
}

for_ThreadList.prototype.drawNewLabels=function(us) {
	this.vids=new Array();
	this.vtms=new Array();
	var str=cookieGet(this.threadlistcookiename);
	if (str) for (i=0;i<str.length-15;i+=16) {
		this.vids.push(parseInt(str.substring(i,i+8),16));
		this.vtms.push(parseInt(str.substring(i+8,i+16),16));
	}
	for (var i=0; i<this.thids.length; ++i) {
		seen=true;
   		if ((us.ok)&&(us.visited>0)&&(this.thtms[i]>us.last))
	    {
   			seen=false;
   			for (var j=0;j<this.vids.length;j++) if ((this.thids[i]==this.vids[j])&&(this.vtms[j]>=this.thtms[i])) {seen=true;}
	    }
		document.getElementById('CH_new'+this.thids[i]).innerHTML=(seen?'':'<img src="/img/new.gif" border="0" alt="New post! (click to mark as read)" title="New post! (click to mark as read)" onclick="'+this.objName+'.mark('+this.thids[i]+')">');
	}
}
for_ThreadList.prototype.mark=function(hid) {
	var qk=cookieGet (this.threadlistcookiename);
	if (!qk) qk='';
	var what=hid.toString(16); while (what.length<8) what='0'+what;
	var when=this.now.toString(16); while (when.length<8) when='0'+when; 
	qk+=what+when;
	cookieSet (this.threadlistcookiename,qk);
	this.cb_drawNewLabels.invoke();
}
for_ThreadList.prototype.drawBookmarks=function(us){xhrAsync(this.httpbookmark+'?w=get&tt='+this.thids.join('+'),bindFunction(this,this.drawBookmarksXHR));}
for_ThreadList.prototype.drawBookmarksXHR=function(jsonArray) {
	try {var ret=JSON.parse(jsonArray);} catch (e) {return;}	// silent behavior (auto-initiated)
	if (ret.success) for (i=0;i<ret.data.length;i++) {this.drawOneBookmark(ret.data[i]);}
	else for (i=0;i<this.thids.length;++i) {this.drawOneBookmark({"threadid":this.thids[i]});}
	return;
}
for_ThreadList.prototype.drawOneBookmark=function(data) {
	document.getElementById('CH_thread'+data.threadid).className=data.participated?'box participated':'box';
	document.getElementById('CH_icos'+data.threadid).innerHTML='';
	document.getElementById('CH_bm'+data.threadid).innerHTML='';
	icos='';
	el=document.getElementById('CH_bm'+data.threadid);
	if (data.participated) {
		document.getElementById('CH_thread'+data.threadid).className='box participated';
		document.getElementById('CH_thread'+data.threadid).title='You have participated in this topic';
	}
	if (data.notify) {
		el.appendChild(btn('Unsubscribe','Remove subscription','mailno','javascript:'+this.objName+'.bookmark('+data.threadid+',-1)'));
	}
	if (data.bookmark) {
		el.appendChild(btn('Remove','Remove bookmark','bookmark','javascript:'+this.objName+'.bookmark('+data.threadid+',-1)'));
	}
	document.getElementById('CH_icos'+data.threadid).innerHTML=icos;
}
for_ThreadList.prototype.bookmark=function(threadid,notify) {
	document.getElementById('CH_bm'+threadid).innerHTML='Please wait...';
	xhrAsync ( this.httpbookmark+'?w=bookmark&t='+threadid+'&q='+notify, bindFunction(this,this.drawBookmarksXHR) );
}


//---------------------------------------------------------------------------
// for_PostList: Manages the "Reply" button, plus creates the for_Post objects
//* better as object (new for_PostList), but can also be called as function
function for_PostList(threadid,threadarchived,threadauthor,threadsolved,elidPosts,elidSubscription,elidReply,elidInsNew,elidAuthor,httpthreads,httpposts,httpbookmark,accesspost,accessreport,accessfilter,accesstrusted,accessmod,accessbookmark) {
	this.threadid=threadid;
	this.threadarchived=threadarchived;
	this.threadauthor=threadauthor;
	this.threadsolved=threadsolved;
	this.elidPosts=elidPosts;
	this.elidSubscription=elidSubscription;
	this.elidReply=elidReply;
	this.elidInsNew=elidInsNew;
	this.elidAuthor=elidAuthor;
	this.httpthreads=httpthreads;
	this.httpposts=httpposts;
	this.httpbookmark=httpbookmark;
	this.accesspost=accesspost;
	this.accessfilter=accessfilter;
	this.accesstrusted=accesstrusted;
	this.accessmod=accessmod;
	this.accessbookmark=accessbookmark;
	this.objName='thread'+threadid;
	eval(this.objName+'=this;');
	
	if (elidPosts) {
		var elPosts=new Array();
		var el=document.getElementById(elidPosts);
		if ( (el) && (el.hasChildNodes) ) {
			for (var i=0; i<el.childNodes.length; ++i) {
				if ( (' '+el.childNodes[i].className+' ').indexOf('C_forPost') != -1) {
					elPosts.push(el.childNodes[i]);
				}
			}
		}
		for (i=0;i<elPosts.length;i++) {
			if ((i==elPosts.length-1)&&(elidReply)) new for_Post (elPosts[i],httpposts,accesspost,accessreport,accessfilter,accesstrusted,accessmod,threadarchived,true);	// last post in thread
			else new for_Post (elPosts[i],httpposts,accesspost,accessreport,accessfilter,accesstrusted,accessmod,threadarchived);
		}
	}
	if (elidSubscription) {
		this.elSubscription=document.getElementById(elidSubscription);
		onSession(this.drawSubscription,this);
	}
	if (elidReply) {	// only one object with reply button, please (it can be the window object)
		this.elReply=document.getElementById(this.elidReply);
		this.originalReply=this.elReply.innerHTML;
		this.cb_drawReply=onSession(this.drawReply,this);
	}
	if ((!this.threadarchived)&&(this.elidAuthor)) { this.cb_drawAuthorOptions=onSession(this.drawAuthorOptions,this); }
}

for_PostList.prototype.drawAuthorOptions=function(us) {
	var el=document.getElementById(this.elidAuthor);
	if (!el) return;
	while (el.hasChildNodes()) el.removeChild(el.lastChild);	// clear element
	if ((this.threadarchived)&&(!us.auth(this.accessmod))) return;
	if (us.auth(this.accessmod)||(us.userid==this.threadauthor)) {
		el.appendChild(btn('Edit topic','Edit this topic','edit',this.httpthreads+'?w=edit&t='+this.threadid));
		if (this.threadsolved) el.appendChild(btn('Remove solved','Removed solved mark','delete','javascript:'+this.objName+'.solved(0)'));
		else el.appendChild(btn('Mark as solved','Mark this topic as solved','solved','javascript:'+this.objName+'.solved(1)'));
	}
}
for_PostList.prototype.solved=function(n) { xhrAsync(this.httpthreads+'?w=solved&t='+this.threadid+'&n='+n,bindFunction(this,this.solvedXHR)); }
for_PostList.prototype.solvedXHR=function(text) {
	try {var ret=JSON.parse(text);} catch (e) {alert(text);return;}
	if (ret.success) {
		this.threadsolved=ret.solved;
		this.cb_drawAuthorOptions.invoke();	// just for the case it fails to reload
		window.location.href=window.location.href;
	}
}

for_PostList.prototype.drawReply=function(us) {
	if ( (!this.threadarchived) && (us.auth(this.accesspost)) ) {
		while (this.elReply.hasChildNodes()) this.elReply.removeChild(this.elReply.lastChild);	// clear element
		this.elButton=this.elReply.appendChild(btn('Reply','Post a new reply','postreply','javascript:'+this.objName+'.reply()'));
	}
	else 
		this.elReply.innerHTML=this.originalReply;
}
for_PostList.prototype.reply=function() {
	if (this.elButton) { this.elButton.parentNode.removeChild(this.elButton); this.elButton=null; }	// clear button
	new BBEdit(this.elReply,{"content":""},this.httpposts,{"w":"reply","t":this.threadid},bindFunction(this,this.replyXHR));
}
for_PostList.prototype.replyXHR=function(text) {	// XHR must enclose post in id=msg# topmost item
	if (text) {	// no text on cancel
		if (text.substr(0,5).toLowerCase()=='error') {alert(text); return;}
		var eldiv = document.createElement("div"); eldiv.innerHTML=text; eldiv=document.getElementById(this.elidInsNew).appendChild(eldiv);
		if (eldiv.hasChildNodes)
			new for_Post (eldiv.firstChild,this.httpposts,this.accesspost,this.accessreport,this.accessfilter,this.accesstrusted,this.accessmod,this.threadarchived,true);
	}
	this.cb_drawReply.invoke();
	WhenAll();
}

for_PostList.prototype.drawSubscription=function(us){
//	if ( ((!this.threadarchived)||(us.auth(this.accessmod))) && ...
	if ((us.ok)&&(us.auth(this.accessbookmark))) {
		this.elSubscription.innerHTML='Loading tracking options...';
		xhrAsync(this.httpbookmark+'?w=get&tt='+this.threadid,bindFunction(this,this.drawSubscriptionXHR));
	}
	else this.elSubscription.innerHTML='';	// or whatever
}
for_PostList.prototype.drawSubscriptionXHR=function(jsonArray) {
	try {var ret=JSON.parse(jsonArray);} catch (e) {alert(jsonArray);return;}
	if (ret.success) {
		data=ret.data[0];
		tx='<form name="fm">Tracking options for this topic: <select name="bmk" onchange="'+this.objName+'.bookmark(fm.bmk.options[fm.bmk.selectedIndex].value,fm.bmk.options[fm.bmk.selectedIndex].text)">';
		tx+='<option value=""></option>';
	    if (data.bookmark) tx+='<option value="-1">Remove bookmark</option>';
	    	else tx+='<option value="0">Bookmark (Add to "my topics" list)</option>';
	    if (data.notify) tx+='<option value="-1">Unsubscribe</option>';
	    	else if (!this.threadarchived) tx+='<option value="1">Subscribe (Receive an email on new replies)</option>';
	    tx+='</select></form>';
	   this.elSubscription.innerHTML=tx;
	}
}
for_PostList.prototype.bookmark=function(notify,question) {
	if (confirm ('Are you sure do you want to '+question+'?')) {
		this.elSubscription.innerHTML='Please wait...';
		xhrAsync ( this.httpbookmark+'?w=bookmark&t='+this.threadid+'&q='+notify, bindFunction(this,this.drawSubscriptionXHR) );
	}
}

//---------------------------------------------------------------------------
// for_Post: Object that manages Editing and Moderating the posts
function for_Post(el,httpcgi,accesspost,accessreport,accessfilter,accesstrusted,accessmod,threadarchived,lastpostinthread) {
	this.el=el;	this.httpcgi=httpcgi; this.accesspost=accesspost; this.accessreport=accessreport; this.accessfilter=accessfilter; this.accesstrusted=accesstrusted;	this.accessmod=accessmod; this.threadarchived=threadarchived; this.lastpostinthread=lastpostinthread;
	this.parseHTML();
	this.objName='post'+this.postid;
	eval(this.objName+'=this;');
	this.cb_onSession=onSession(this.onSession,this);
}

for_Post.prototype.parseHTML=function () {	// this would be onPrint, but called by ctor
	if (this.el.hasChildNodes()) {
		var data=this.el.firstChild.title.split(',');
		this.postid=data[0];
		this.author=data[1];
		this.authoraccess=data[2];
		this.authornposts=data[3];
		this.poststatus=data[4];
		this.eledtEdit=document.getElementById('CH_i'+this.postid);
		this.elButtons=document.getElementById('CH_btn'+this.postid);
	}
}

for_Post.prototype.onSession= function (session) {
	var b=this.elButtons;
	if (b) {
		while (b.hasChildNodes()) {b.removeChild(b.lastChild);}		// clear element
		if ((this.threadarchived)&&(!session.auth(this.accessmod))) return;
		if (session.ok) {
			if ( (session.auth(this.accessmod)) || ((session.auth(this.accesspost))&&(session.userid==this.author)) ) b.appendChild(btn('Edit','Edit post','edit','javascript:'+this.objName+'.edit()','narrow'));
			if (this.poststatus==-1) {		// reported
				if (session.auth(this.accessmod)) {
					b.appendChild(btn('Approve','Approve post','stamp','javascript:'+this.objName+'.approve()','narrow'));
				}
				else b.appendChild(btn('Reported','This message has been reported','reportoff',false,'narrow'));
			}
			if ( ((this.authoraccess&this.accesstrusted)!=this.accesstrusted) && (this.poststatus<1) && (session.auth(this.accessfilter)) && (!session.auth(this.accessmod)) )
				b.appendChild(btn('Report','Report post as abusive (this will delete the post)','report','javascript:'+this.objName+'.filter()','narrow'));
			else if ( (this.poststatus>=0) && (session.auth(this.accessreport)) ) b.appendChild(btn('Report','Report post as abusive','report','javascript:'+this.objName+'.report()','narrow'));
			if ( (session.auth(this.accessmod))
					|| (this.lastpostinthread && (session.userid==this.author)) )
				b.appendChild(btn('Delete','Delete post','delete','javascript:'+this.objName+'.del()','narrow'));
		}
	}	
}

for_Post.prototype.edit=function() {
	this.backup=this.eledtEdit.innerHTML;
	this.eledtEdit.innerHTML=T('Loading data. Please wait...');
	this.elButtons.style.display='none';	// remove buttons
	
	var onGotten=bindFunction(this,function(text) {
		this.eledtEdit.innerHTML='';
		new BBEdit(this.eledtEdit,{"content":text},this.httpcgi,{"w":"edit","p":this.postid},bindFunction(this,this.editXHR));		
	});
	
	xhrAsync (this.httpcgi+'?w=text&p='+this.postid, onGotten );
}

for_Post.prototype.editXHR=function(text) {
	if ( (!text) || (!this.redraw(text)) ) {
		this.eledtEdit.innerHTML=this.backup;
		this.elButtons.style.display='';
	}
}

for_Post.prototype.report=function() {
	var reason=prompt(T('REPORTING ABUSE: Please, describe the problem with this message:'),'');
	if (reason) xhrAsync (this.httpcgi+'?w=report&p='+this.postid+'&why='+escape(reason), bindFunction(this,this.redraw) );
}
for_Post.prototype.approve=function() {
	var reason=prompt(T('APPROVE MESSAGE: Please, enter the reason (optional):'),'');
	xhrAsync (this.httpcgi+'?w=approve&p='+this.postid+'&why='+escape(reason), bindFunction(this,this.redraw) );
}
for_Post.prototype.filter=function() {
	var reason=prompt(T('WARNING: This action will remove this post from the forum until a moderator can check at the issue.\n\n Please, describe the problem with this message:'),'');
	if (reason) xhrAsync ( this.httpcgi+'?w=filter&p='+this.postid+'&why='+escape(reason), bindFunction(this,this.redraw) );
}
for_Post.prototype.del=function() {
	if (confirm('You are about to delete a post')) xhrAsync ( this.httpcgi+'?w=del&p='+this.postid, bindFunction(this,this.redraw) );
}

for_Post.prototype.redraw=function(text) {
	if (text) {
		if (text.substr(0,5).toLowerCase()=='error') {alert(text); return false;}
		else {
			// special trick: rewrite entire element:
			var eldiv=document.createElement('div'); eldiv.innerHTML=text;
			var elnew=eldiv.firstChild;
			this.el.parentNode.replaceChild(elnew,this.el);
			this.el=elnew;
			this.parseHTML();
			this.cb_onSession.invoke();
			WhenAll();
			return true;
		}
	}
}
