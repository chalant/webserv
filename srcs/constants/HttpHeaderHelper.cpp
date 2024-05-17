#include "../../includes/constants/HttpHeaderHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * HttpVersionHelper.cpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP versions.
 * It defines an enum HttpVersion to represent standard HTTP versions and provides methods
 * to retrieve string representations of versions and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpHeaderHelper::HttpHeaderHelper()
    : _headerList(_setHeaderList()),
      _stringHttpHeaderMap(_setStringHttpHeaderMap()),
      _httpHeaderStringMap(_setHttpHeaderStringMap()) {}

// Get string representation of HttpHeader enum value
const std::string &HttpHeaderHelper::httpHeaderStringMap(HttpHeader header) const
{
    if (_httpHeaderStringMap.find(header) != _httpHeaderStringMap.end())
    {
        return _httpHeaderStringMap.at(header);
    }
    else
    {
        throw UnknownHeaderError();
    }
}

// Get HttpHeader enum value from string representation
HttpHeader HttpHeaderHelper::stringHttpHeaderMap(const std::string &header) const
{
    if (_stringHttpHeaderMap.find(header) != _stringHttpHeaderMap.end())
    {
        return _stringHttpHeaderMap.at(header);
    }
    else
    {
        throw UnknownHeaderError(header);
    }
}

// Check if a string is a valid HTTP header name
bool HttpHeaderHelper::isHeaderName(const std::string &header) const
{
    return _stringHttpHeaderMap.find(header) != _stringHttpHeaderMap.end();
}

// Helper function to initialize headerList with string representations of HTTP headers
std::vector<std::string> HttpHeaderHelper::_setHeaderList()
{
    std::vector<std::string> headerList;

    // Add string representations of HTTP headers to headerList
    headerList.push_back("cache-control");
    headerList.push_back("connection");
    headerList.push_back("date");
    headerList.push_back("pragma");
    headerList.push_back("trailer");
    headerList.push_back("transfer-encoding");
    headerList.push_back("upgrade");
    headerList.push_back("via");
    headerList.push_back("warning");
    headerList.push_back("accept");
    headerList.push_back("accept-charset");
    headerList.push_back("accept-encoding");
    headerList.push_back("accept-language");
    headerList.push_back("authorization");
    headerList.push_back("expect");
    headerList.push_back("from");
    headerList.push_back("host");
    headerList.push_back("if-match");
    headerList.push_back("if-modified-since");
    headerList.push_back("if-none-match");
    headerList.push_back("if-range");
    headerList.push_back("if-unmodified-since");
    headerList.push_back("max-forwards");
    headerList.push_back("proxy-authorization");
    headerList.push_back("range");
    headerList.push_back("referer");
    headerList.push_back("te");
    headerList.push_back("user-agent");
    headerList.push_back("accept-ranges");
    headerList.push_back("age");
    headerList.push_back("etag");
    headerList.push_back("location");
    headerList.push_back("proxy-authenticate");
    headerList.push_back("retry-after");
    headerList.push_back("server");
    headerList.push_back("vary");
    headerList.push_back("www-authenticate");
    headerList.push_back("allow");
    headerList.push_back("content-encoding");
    headerList.push_back("content-language");
    headerList.push_back("content-length");
    headerList.push_back("content-location");
    headerList.push_back("content-md5");
    headerList.push_back("content-range");
    headerList.push_back("content-type");
    headerList.push_back("expires");
    headerList.push_back("last-modified");
    headerList.push_back("access-control-allow-origin");
    headerList.push_back("access-control-allow-methods");
    headerList.push_back("access-control-allow-headers");
    headerList.push_back("access-control-max-age");
    headerList.push_back("access-control-allow-credentials");
    headerList.push_back("access-control-expose-headers");
    headerList.push_back("access-control-request-method");
    headerList.push_back("access-control-request-headers");
    headerList.push_back("content-disposition");
    headerList.push_back("content-security-policy");
    headerList.push_back("content-security-policy-report-only");
    headerList.push_back("dnt");
    headerList.push_back("forwarded");
    headerList.push_back("origin");
    headerList.push_back("timing-allow-origin");
    headerList.push_back("x-content-type-options");
    headerList.push_back("x-frame-options");
    headerList.push_back("x-xss-protection");
    headerList.push_back("x-forwarded-for");
    headerList.push_back("cookie");
    headerList.push_back("set-cookie");
    headerList.push_back("upgrade-insecure-requests");

    return headerList;
}

// Helper function to initialize stringHttpHeaderMap with string representations of HTTP headers
std::map<std::string, HttpHeader> HttpHeaderHelper::_setStringHttpHeaderMap()
{
    std::map<std::string, HttpHeader> stringHttpHeaderMap;

    // Add mappings from string representations to HttpHeader enum values
    stringHttpHeaderMap["cache-control"] = CACHE_CONTROL;
    stringHttpHeaderMap["connection"] = CONNECTION;
    stringHttpHeaderMap["date"] = DATE;
    stringHttpHeaderMap["pragma"] = PRAGMA;
    stringHttpHeaderMap["trailer"] = TRAILER;
    stringHttpHeaderMap["transfer-encoding"] = TRANSFER_ENCODING;
    stringHttpHeaderMap["upgrade"] = UPGRADE;
    stringHttpHeaderMap["via"] = VIA;
    stringHttpHeaderMap["warning"] = WARNING;
    stringHttpHeaderMap["accept"] = ACCEPT;
    stringHttpHeaderMap["accept-charset"] = ACCEPT_CHARSET;
    stringHttpHeaderMap["accept-encoding"] = ACCEPT_ENCODING;
    stringHttpHeaderMap["accept-language"] = ACCEPT_LANGUAGE;
    stringHttpHeaderMap["authorization"] = AUTHORIZATION;
    stringHttpHeaderMap["expect"] = EXPECT;
    stringHttpHeaderMap["from"] = FROM;
    stringHttpHeaderMap["host"] = HOST;
    stringHttpHeaderMap["if-match"] = IF_MATCH;
    stringHttpHeaderMap["if-modified-since"] = IF_MODIFIED_SINCE;
    stringHttpHeaderMap["if-none-match"] = IF_NONE_MATCH;
    stringHttpHeaderMap["if-range"] = IF_RANGE;
    stringHttpHeaderMap["if-unmodified-since"] = IF_UNMODIFIED_SINCE;
    stringHttpHeaderMap["max-forwards"] = MAX_FORWARDS;
    stringHttpHeaderMap["proxy-authorization"] = PROXY_AUTHORIZATION;
    stringHttpHeaderMap["range"] = RANGE;
    stringHttpHeaderMap["referer"] = REFERER;
    stringHttpHeaderMap["te"] = TE;
    stringHttpHeaderMap["user-agent"] = USER_AGENT;
    stringHttpHeaderMap["accept-ranges"] = ACCEPT_RANGES;
    stringHttpHeaderMap["age"] = AGE;
    stringHttpHeaderMap["etag"] = ETAG;
    stringHttpHeaderMap["location"] = LOCATION;
    stringHttpHeaderMap["proxy-authenticate"] = PROXY_AUTHENTICATE;
    stringHttpHeaderMap["retry-after"] = RETRY_AFTER;
    stringHttpHeaderMap["server"] = SERVER;
    stringHttpHeaderMap["vary"] = VARY;
    stringHttpHeaderMap["www-authenticate"] = WWW_AUTHENTICATE;
    stringHttpHeaderMap["allow"] = ALLOW;
    stringHttpHeaderMap["content-encoding"] = CONTENT_ENCODING;
    stringHttpHeaderMap["content-language"] = CONTENT_LANGUAGE;
    stringHttpHeaderMap["content-length"] = CONTENT_LENGTH;
    stringHttpHeaderMap["content-location"] = CONTENT_LOCATION;
    stringHttpHeaderMap["content-md5"] = CONTENT_MD5;
    stringHttpHeaderMap["content-range"] = CONTENT_RANGE;
    stringHttpHeaderMap["content-type"] = CONTENT_TYPE;
    stringHttpHeaderMap["expires"] = EXPIRES;
    stringHttpHeaderMap["last-modified"] = LAST_MODIFIED;
    stringHttpHeaderMap["access-control-allow-origin"] = ACCESS_CONTROL_ALLOW_ORIGIN;
    stringHttpHeaderMap["access-control-allow-methods"] = ACCESS_CONTROL_ALLOW_METHODS;
    stringHttpHeaderMap["access-control-allow-headers"] = ACCESS_CONTROL_ALLOW_HEADERS;
    stringHttpHeaderMap["access-control-max-age"] = ACCESS_CONTROL_MAX_AGE;
    stringHttpHeaderMap["access-control-allow-credentials"] = ACCESS_CONTROL_ALLOW_CREDENTIALS;
    stringHttpHeaderMap["access-control-expose-headers"] = ACCESS_CONTROL_EXPOSE_HEADERS;
    stringHttpHeaderMap["access-control-request-method"] = ACCESS_CONTROL_REQUEST_METHOD;
    stringHttpHeaderMap["access-control-request-headers"] = ACCESS_CONTROL_REQUEST_HEADERS;
    stringHttpHeaderMap["content-disposition"] = CONTENT_DISPOSITION;
    stringHttpHeaderMap["content-security-policy"] = CONTENT_SECURITY_POLICY;
    stringHttpHeaderMap["content-security-policy-report-only"] = CONTENT_SECURITY_POLICY_REPORT_ONLY;
    stringHttpHeaderMap["dnt"] = DNT;
    stringHttpHeaderMap["forwarded"] = FORWARDED;
    stringHttpHeaderMap["origin"] = ORIGIN;
    stringHttpHeaderMap["timing-allow-origin"] = TIMING_ALLOW_ORIGIN;
    stringHttpHeaderMap["x-content-type-options"] = X_CONTENT_TYPE_OPTIONS;
    stringHttpHeaderMap["x-frame-options"] = X_FRAME_OPTIONS;
    stringHttpHeaderMap["x-xss-protection"] = X_XSS_PROTECTION;
    stringHttpHeaderMap["x-forwarded-for"] = X_FORWARDED_FOR;
    stringHttpHeaderMap["cookie"] = COOKIE;
    stringHttpHeaderMap["set-cookie"] = SET_COOKIE;
	stringHttpHeaderMap["upgrade-insecure-requests"] = UPGRADE_INSECURE_REQUESTS;

    return stringHttpHeaderMap;
}

// Helper function to initialize httpHeaderStringMap with mappings from HttpHeader enum values to string representations
std::map<HttpHeader, std::string> HttpHeaderHelper::_setHttpHeaderStringMap()
{
    std::map<HttpHeader, std::string> httpHeaderStringMap;

    // Add mappings from HttpHeader enum values to string representations
    httpHeaderStringMap[CACHE_CONTROL] = "cache-control";
    httpHeaderStringMap[CONNECTION] = "connection";
    httpHeaderStringMap[DATE] = "date";
    httpHeaderStringMap[PRAGMA] = "pragma";
    httpHeaderStringMap[TRAILER] = "trailer";
    httpHeaderStringMap[TRANSFER_ENCODING] = "transfer-encoding";
    httpHeaderStringMap[UPGRADE] = "upgrade";
    httpHeaderStringMap[VIA] = "via";
    httpHeaderStringMap[WARNING] = "warning";
    httpHeaderStringMap[ACCEPT] = "accept";
    httpHeaderStringMap[ACCEPT_CHARSET] = "accept-charset";
    httpHeaderStringMap[ACCEPT_ENCODING] = "accept-encoding";
    httpHeaderStringMap[ACCEPT_LANGUAGE] = "accept-language";
    httpHeaderStringMap[AUTHORIZATION] = "authorization";
    httpHeaderStringMap[EXPECT] = "expect";
    httpHeaderStringMap[FROM] = "from";
    httpHeaderStringMap[HOST] = "host";
    httpHeaderStringMap[IF_MATCH] = "if-match";
    httpHeaderStringMap[IF_MODIFIED_SINCE] = "if-modified-since";
    httpHeaderStringMap[IF_NONE_MATCH] = "if-none-match";
    httpHeaderStringMap[IF_RANGE] = "if-range";
    httpHeaderStringMap[IF_UNMODIFIED_SINCE] = "if-unmodified-since";
    httpHeaderStringMap[MAX_FORWARDS] = "max-forwards";
    httpHeaderStringMap[PROXY_AUTHORIZATION] = "proxy-authorization";
    httpHeaderStringMap[RANGE] = "range";
    httpHeaderStringMap[REFERER] = "referer";
    httpHeaderStringMap[TE] = "te";
    httpHeaderStringMap[USER_AGENT] = "user-agent";
    httpHeaderStringMap[ACCEPT_RANGES] = "accept-ranges";
    httpHeaderStringMap[AGE] = "age";
    httpHeaderStringMap[ETAG] = "etag";
    httpHeaderStringMap[LOCATION] = "location";
    httpHeaderStringMap[PROXY_AUTHENTICATE] = "proxy-authenticate";
    httpHeaderStringMap[RETRY_AFTER] = "retry-after";
    httpHeaderStringMap[SERVER] = "server";
    httpHeaderStringMap[VARY] = "vary";
    httpHeaderStringMap[WWW_AUTHENTICATE] = "www-authenticate";
    httpHeaderStringMap[ALLOW] = "allow";
    httpHeaderStringMap[CONTENT_ENCODING] = "content-encoding";
    httpHeaderStringMap[CONTENT_LANGUAGE] = "content-language";
    httpHeaderStringMap[CONTENT_LENGTH] = "content-length";
    httpHeaderStringMap[CONTENT_LOCATION] = "content-location";
    httpHeaderStringMap[CONTENT_MD5] = "content-md5";
    httpHeaderStringMap[CONTENT_RANGE] = "content-range";
    httpHeaderStringMap[CONTENT_TYPE] = "content-type";
    httpHeaderStringMap[EXPIRES] = "expires";
    httpHeaderStringMap[LAST_MODIFIED] = "last-modified";
    httpHeaderStringMap[ACCESS_CONTROL_ALLOW_ORIGIN] = "access-control-allow-origin";
    httpHeaderStringMap[ACCESS_CONTROL_ALLOW_METHODS] = "access-control-allow-methods";
    httpHeaderStringMap[ACCESS_CONTROL_ALLOW_HEADERS] = "access-control-allow-headers";
    httpHeaderStringMap[ACCESS_CONTROL_MAX_AGE] = "access-control-max-age";
    httpHeaderStringMap[ACCESS_CONTROL_ALLOW_CREDENTIALS] = "access-control-allow-credentials";
    httpHeaderStringMap[ACCESS_CONTROL_EXPOSE_HEADERS] = "access-control-expose-headers";
    httpHeaderStringMap[ACCESS_CONTROL_REQUEST_METHOD] = "access-control-request-method";
    httpHeaderStringMap[ACCESS_CONTROL_REQUEST_HEADERS] = "access-control-request-headers";
    httpHeaderStringMap[CONTENT_DISPOSITION] = "content-disposition";
    httpHeaderStringMap[CONTENT_SECURITY_POLICY] = "content-security-policy";
    httpHeaderStringMap[CONTENT_SECURITY_POLICY_REPORT_ONLY] = "content-security-policy-report-only";
    httpHeaderStringMap[DNT] = "dnt";
    httpHeaderStringMap[FORWARDED] = "forwarded";
    httpHeaderStringMap[ORIGIN] = "origin";
    httpHeaderStringMap[TIMING_ALLOW_ORIGIN] = "timing-allow-origin";
    httpHeaderStringMap[X_CONTENT_TYPE_OPTIONS] = "x-content-type-options";
    httpHeaderStringMap[X_FRAME_OPTIONS] = "x-frame-options";
    httpHeaderStringMap[X_XSS_PROTECTION] = "x-xss-protection";
    httpHeaderStringMap[X_FORWARDED_FOR] = "x-forwarded-for";
    httpHeaderStringMap[COOKIE] = "cookie";
    httpHeaderStringMap[SET_COOKIE] = "set-cookie";
	httpHeaderStringMap[UPGRADE_INSECURE_REQUESTS] = "upgrade-insecure-requests";

    return httpHeaderStringMap;
}

// Path: srcs/constants/HttpHeaderHelper.cpp
