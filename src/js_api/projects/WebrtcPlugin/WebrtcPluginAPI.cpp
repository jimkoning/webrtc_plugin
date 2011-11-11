/**********************************************************\

  Auto-generated WebrtcPluginAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "WebrtcPluginAPI.h"
#include "WPLPeerConnectionClient.h"
#include "WPLSocketServer.h"
#include "talk/base/thread.h"
#include "rtc_common.h"


void MainThreadRunner(void)
{
    talk_base::Thread::Current()->Run();
    talk_base::Thread::Current()->set_socketserver(NULL);
}

int PluginMainThread::workerBee(void)
{
    talk_base::AutoThread autoThread;

    projectname::SocketServer socketServer;
    talk_base::Thread::Current()->set_socketserver(&socketServer);
    
    projectname::PeerConnectionClient testClient(m_pMsgQ, "", "", -1);
    socketServer.SetPeerConnectionClient(&testClient);

    talk_base::Thread::Current()->Run();
    talk_base::Thread::Current()->set_socketserver(NULL);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcPluginAPI::WebrtcPluginAPI(const WebrtcPluginPtr& plugin, const FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
WebrtcPluginAPI::WebrtcPluginAPI(const WebrtcPluginPtr& plugin, const FB::BrowserHostPtr& host) : m_plugin(plugin), m_host(host)
{
    registerMethod("echo",      make_method(this, &WebrtcPluginAPI::echo));
    registerMethod("testEvent", make_method(this, &WebrtcPluginAPI::testEvent));
    registerMethod("Signin", make_method(this, &WebrtcPluginAPI::Signin));
    registerMethod("Signout", make_method(this, &WebrtcPluginAPI::Signout));

    // Read-write property
    registerProperty("testString",
                     make_property(this,
                        &WebrtcPluginAPI::get_testString,
                        &WebrtcPluginAPI::set_testString));

    // Read-only property
    registerProperty("version",
                     make_property(this,
                        &WebrtcPluginAPI::get_version));
    
    m_testString = "Hello World";
    
    m_pMsgQ = new (projectname::ThreadSafeMessageQueue)();
    PluginMainThread *pmt = new PluginMainThread(m_pMsgQ);
    pmt->startThread();
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcPluginAPI::~WebrtcPluginAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
WebrtcPluginAPI::~WebrtcPluginAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcPluginPtr WebrtcPluginAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
WebrtcPluginPtr WebrtcPluginAPI::getPlugin()
{
    WebrtcPluginPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}



// Read/Write property testString
std::string WebrtcPluginAPI::get_testString()
{
    return m_testString;
}
void WebrtcPluginAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string WebrtcPluginAPI::get_version()
{
    return "CURRENT_VERSION";
}

// Method echo
FB::variant WebrtcPluginAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo(msg, n++);
    return msg;
}

void WebrtcPluginAPI::testEvent(const FB::variant& var)
{
    fire_fired(var, true, 1);
}

void WebrtcPluginAPI::Signin(const std::string& peerName,
                             const std::string& serverIP,
                             const int serverPort)
{
    ParsedCommand cmd;

    cmd["command"] = "signin";
    cmd["server"] = serverIP,
    cmd["peername"] = peerName,
    cmd["serverport"] = ::ToString(serverPort);
    m_pMsgQ->PostMessage(cmd);
}

void WebrtcPluginAPI::Signout(void)
{
    ParsedCommand cmd;
    
    cmd["command"] = "signout";
    m_pMsgQ->PostMessage(cmd);
}