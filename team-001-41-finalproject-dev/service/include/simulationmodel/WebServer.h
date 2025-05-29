/**
 * AI Acknowledgement: Anthropic Claude 3.7 Sonnet
 * - Used for assistence with Doxygen Commenting
 */

/**
Copyright (c) 2019 Dan Orban
*/

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <map>
#include <string>
#include <vector>

#include "libwebsockets.h"
#include "libwebsockets/lws-service.h"

/**
 * @brief Base class for web servers
 *
 * Provides core functionality for setting up and managing a web server with
 * WebSocket support
 */
class WebServerBase {
 public:
  /**
   * @brief Constructs a web server
   * @param port Port number for the server to listen on (default: 8081)
   * @param webDir Directory containing web files to serve (default: current
   * directory)
   */
  WebServerBase(int port = 8081, const std::string& webDir = ".");

  /**
   * @brief Destructor for Web Server
   */
  virtual ~WebServerBase();

  /**
   * @brief Class representing a WebSocket session with a client
   */
  class Session {
    friend class WebServerBase;

   public:
    /**
     * @brief Constructs a new session with a unique ID
     */
    Session();

    /**
     * @brief Destructor for Session
     */
    virtual ~Session();

    /**
     * @brief Gets the unique ID for this session
     * @return The session ID
     */
    virtual int getId() const { return id; }

    /**
     * @brief Handles a message received from the client
     * @param msg The message content
     */
    virtual void receiveMessage(const std::string& msg) {}

    /**
     * @brief Sends a message to the client
     * @param msg The message to send
     */
    virtual void sendMessage(const std::string& msg);

    /**
     * @brief Called periodically to update session state
     */
    virtual void update() {}

    /**
     * @brief Handles the low-level write operation to send pending messages
     */
    virtual void onWrite();

   private:
    void* state;
    int id;
  };

  /**
   * @brief Services pending server events
   * @param time Maximum time in milliseconds to spend processing events
   * (default: 10)
   */
  void service(int time = 10);

  /**
   * @brief Creates a session from websocket initialization data
   * @param info Pointer to the websocket session data
   */
  virtual void createSession(void* info);

 protected:
  /**
   * @brief Factory method to create a new session
   * @return A newly created Session object
   */
  virtual Session* createSession() { return new Session(); }

 public:
  lws_context* context = nullptr;
  std::vector<Session*> sessions;
  std::map<int, Session*> sessionMap;
  std::string webDir;
};

/**
 * @brief Template class for creating web servers with custom session types
 * @tparam T The custom session class which must inherit from
 * WebServerBase::Session
 */
template <typename T>
class WebServer : public WebServerBase {
 public:
  /**
   * @brief Constructs a WebServer with a custom session type
   * @param port Port number for the server to listen on (default: 8081)
   * @param webDir Directory containing web files to serve (default: current
   * directory)
   */
  WebServer(int port = 8081, const std::string& webDir = ".")
      : WebServerBase(port, webDir) {}

 protected:
  /**
   * @brief Factory method to create a session of type T
   * @return A newly created session of type T
   */
  Session* createSession() { return new T(); }
};

/**
 * @brief Template class for creating web servers with custom session types and
 * shared state
 * @tparam T The custom session class which must inherit from
 * WebServerBase::Session
 * @tparam STATE The type of the state object to be shared among sessions
 */
template <typename T, typename STATE>
class WebServerWithState : public WebServerBase {
 public:
  /**
   * @brief Constructs a WebServer with a custom session type and shared state
   * @param state The state object to share among sessions
   * @param port Port number for the server to listen on (default: 8081)
   * @param webDir Directory containing web files to serve (default: current
   * directory)
   */
  WebServerWithState(STATE state, int port = 8081,
                     const std::string& webDir = ".")
      : WebServerBase(port, webDir), state(state) {}

 protected:
  /**
   * @brief Factory method to create a session of type T with shared state
   * @return A newly created session of type T initialized with the shared state
   */
  Session* createSession() { return new T(state); }

 private:
  STATE state;
};

// JSON support
#include "picojson.h"

/**
 * @brief Session class with JSON message handling capabilities
 */
class JSONSession : public WebServerBase::Session {
 public:
  /**
   * @brief Handles a JSON message from the client
   * @param val The parsed JSON value
   */
  virtual void receiveJSON(picojson::value& val) {}

  /**
   * @brief Sends a JSON message to the client
   * @param val The JSON value to send
   */
  virtual void sendJSON(picojson::value& val) { sendMessage(val.serialize()); }

  /**
   * @brief Handles a raw message by parsing it as JSON
   * @param msg The raw message string
   *
   * Accumulates partial messages until a complete JSON object is received
   */
  void receiveMessage(const std::string& msg) {
    static std::string buf = "";
    picojson::value val;
    std::string err = picojson::parse(val, msg);
    if (err.empty() && val.is<picojson::object>()) {
      buf = "";
      receiveJSON(val);
    } else {
      buf += msg;
      err = picojson::parse(val, buf);
      if (err.empty() && val.is<picojson::object>()) {
        buf = "";
        receiveJSON(val);
      }
    }
  }
};

#include "util/json.h"
/**
 * @brief Session class for handling command-based JSON messaging
 */
class JsonSession : public JSONSession {
 public:
  /**
   * @brief Receive a command from the web server
   * @param val: the command (in JSON format)
   */
  void receiveJSON(picojson::value& val) {
    JsonObject data = JsonValue(val);

    std::string cmd = data["command"];

    JsonObject returnValue;
    returnValue["id"] = data["id"];

    /**
     * @brief Processes a JSON command from the client
     * @param val The parsed JSON value containing the command
     *
     * Extracts command and data from the JSON, processes it through
     * receiveCommand, and sends back the return value.
     */
    receiveCommand(cmd, data, returnValue);
    JsonValue retVal(returnValue);
    sendJSON(retVal.getValue());
  }

  /**
   * @brief Handles specific commands from the web server
   * @param cmd: command
   * @param data: data (JSON)
   * @param returnValue: return value (JSON) which will be sent back to the
   * server
   */
  virtual void receiveCommand(const std::string& cmd, const JsonObject& data,
                              JsonObject& returnValue) = 0;
};

#endif  // WEBSERVER_H_
