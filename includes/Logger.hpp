/* Logger Class provides functionality to log errors and access events to separate files.
 * Default log files could be "./error.log" and "./access.log"; which can be overridden through the config file.
 * It should support logging with customizable log levels. Error and access logs should be handled separately.
 * Log messages should not immediately be written to the log files but be deferred until after poll() returns, 
 * and done in a non-blocking way so as not to hold up the main thread.
 */