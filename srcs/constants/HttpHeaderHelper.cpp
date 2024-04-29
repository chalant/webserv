#include "constants/HttpHeaderHelper.hpp"

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

    return headerList;
}

// Helper function to initialize stringHttpHeaderMap with string representations of HTTP headers
std::map<std::string, HttpHeader> HttpHeaderHelper::_setStringHttpHeaderMap()
{
    std::map<std::string, HttpHeader> stringHttpHeaderMap;

    // Add mappings from string representations to HttpHeader enum values
    stringHttpHeaderMap["cache-control"] = HttpHeader::CACHE_CONTROL;
    stringHttpHeaderMap["connection"] = HttpHeader::CONNECTION;
    stringHttpHeaderMap["date"] = HttpHeader::DATE;
    stringHttpHeaderMap["pragma"] = HttpHeader::PRAGMA;
    stringHttpHeaderMap["trailer"] = HttpHeader::TRAILER;
    stringHttpHeaderMap["transfer-encoding"] = HttpHeader::TRANSFER_ENCODING;
    stringHttpHeaderMap["upgrade"] = HttpHeader::UPGRADE;
    stringHttpHeaderMap["via"] = HttpHeader::VIA;
    stringHttpHeaderMap["warning"] = HttpHeader::WARNING;
    stringHttpHeaderMap["accept"] = HttpHeader::ACCEPT;
    stringHttpHeaderMap["accept-charset"] = HttpHeader::ACCEPT_CHARSET;
    stringHttpHeaderMap["accept-encoding"] = HttpHeader::ACCEPT_ENCODING;
    stringHttpHeaderMap["accept-language"] = HttpHeader::ACCEPT_LANGUAGE;
    stringHttpHeaderMap["authorization"] = HttpHeader::AUTHORIZATION;
    stringHttpHeaderMap["expect"] = HttpHeader::EXPECT;
    stringHttpHeaderMap["from"] = HttpHeader::FROM;
    stringHttpHeaderMap["host"] = HttpHeader::HOST;
    stringHttpHeaderMap["if-match"] = HttpHeader::IF_MATCH;
    stringHttpHeaderMap["if-modified-since"] = HttpHeader::IF_MODIFIED_SINCE;
    stringHttpHeaderMap["if-none-match"] = HttpHeader::IF_NONE_MATCH;
    stringHttpHeaderMap["if-range"] = HttpHeader::IF_RANGE;
    stringHttpHeaderMap["if-unmodified-since"] = HttpHeader::IF_UNMODIFIED_SINCE;
    stringHttpHeaderMap["max-forwards"] = HttpHeader::MAX_FORWARDS;
    stringHttpHeaderMap["proxy-authorization"] = HttpHeader::PROXY_AUTHORIZATION;
    stringHttpHeaderMap["range"] = HttpHeader::RANGE;
    stringHttpHeaderMap["referer"] = HttpHeader::REFERER;
    stringHttpHeaderMap["te"] = HttpHeader::TE;
    stringHttpHeaderMap["user-agent"] = HttpHeader::USER_AGENT;
    stringHttpHeaderMap["accept-ranges"] = HttpHeader::ACCEPT_RANGES;
    stringHttpHeaderMap["age"] = HttpHeader::AGE;
    stringHttpHeaderMap["etag"] = HttpHeader::ETAG;
    stringHttpHeaderMap["location"] = HttpHeader::LOCATION;
    stringHttpHeaderMap["proxy-authenticate"] = HttpHeader::PROXY_AUTHENTICATE;
    stringHttpHeaderMap["retry-after"] = HttpHeader::RETRY_AFTER;
    stringHttpHeaderMap["server"] = HttpHeader::SERVER;
    stringHttpHeaderMap["vary"] = HttpHeader::VARY;
    stringHttpHeaderMap["www-authenticate"] = HttpHeader::WWW_AUTHENTICATE;
    stringHttpHeaderMap["allow"] = HttpHeader::ALLOW;
    stringHttpHeaderMap["content-encoding"] = HttpHeader::CONTENT_ENCODING;
    stringHttpHeaderMap["content-language"] = HttpHeader::CONTENT_LANGUAGE;
    stringHttpHeaderMap["content-length"] = HttpHeader::CONTENT_LENGTH;
    stringHttpHeaderMap["content-location"] = HttpHeader::CONTENT_LOCATION;
    stringHttpHeaderMap["content-md5"] = HttpHeader::CONTENT_MD5;
    stringHttpHeaderMap["content-range"] = HttpHeader::CONTENT_RANGE;
    stringHttpHeaderMap["content-type"] = HttpHeader::CONTENT_TYPE;
    stringHttpHeaderMap["expires"] = HttpHeader::EXPIRES;
    stringHttpHeaderMap["last-modified"] = HttpHeader::LAST_MODIFIED;
    stringHttpHeaderMap["access-control-allow-origin"] = HttpHeader::ACCESS_CONTROL_ALLOW_ORIGIN;
    stringHttpHeaderMap["access-control-allow-methods"] = HttpHeader::ACCESS_CONTROL_ALLOW_METHODS;
    stringHttpHeaderMap["access-control-allow-headers"] = HttpHeader::ACCESS_CONTROL_ALLOW_HEADERS;
    stringHttpHeaderMap["access-control-max-age"] = HttpHeader::ACCESS_CONTROL_MAX_AGE;
    stringHttpHeaderMap["access-control-allow-credentials"] = HttpHeader::ACCESS_CONTROL_ALLOW_CREDENTIALS;
    stringHttpHeaderMap["access-control-expose-headers"] = HttpHeader::ACCESS_CONTROL_EXPOSE_HEADERS;
    stringHttpHeaderMap["access-control-request-method"] = HttpHeader::ACCESS_CONTROL_REQUEST_METHOD;
    stringHttpHeaderMap["access-control-request-headers"] = HttpHeader::ACCESS_CONTROL_REQUEST_HEADERS;
    stringHttpHeaderMap["content-disposition"] = HttpHeader::CONTENT_DISPOSITION;
    stringHttpHeaderMap["content-security-policy"] = HttpHeader::CONTENT_SECURITY_POLICY;
    stringHttpHeaderMap["content-security-policy-report-only"] = HttpHeader::CONTENT_SECURITY_POLICY_REPORT_ONLY;
    stringHttpHeaderMap["dnt"] = HttpHeader::DNT;
    stringHttpHeaderMap["forwarded"] = HttpHeader::FORWARDED;
    stringHttpHeaderMap["origin"] = HttpHeader::ORIGIN;
    stringHttpHeaderMap["timing-allow-origin"] = HttpHeader::TIMING_ALLOW_ORIGIN;
    stringHttpHeaderMap["x-content-type-options"] = HttpHeader::X_CONTENT_TYPE_OPTIONS;
    stringHttpHeaderMap["x-frame-options"] = HttpHeader::X_FRAME_OPTIONS;
    stringHttpHeaderMap["x-xss-protection"] = HttpHeader::X_XSS_PROTECTION;
    stringHttpHeaderMap["x-forwarded-for"] = HttpHeader::X_FORWARDED_FOR;
    stringHttpHeaderMap["cookie"] = HttpHeader::COOKIE;
    stringHttpHeaderMap["set-cookie"] = HttpHeader::SET_COOKIE;

    return stringHttpHeaderMap;
}

// Helper function to initialize httpHeaderStringMap with mappings from HttpHeader enum values to string representations
std::map<HttpHeader, std::string> HttpHeaderHelper::_setHttpHeaderStringMap()
{
    std::map<HttpHeader, std::string> httpHeaderStringMap;

    // Add mappings from HttpHeader enum values to string representations
    httpHeaderStringMap[HttpHeader::CACHE_CONTROL] = "cache-control";
    httpHeaderStringMap[HttpHeader::CONNECTION] = "connection";
    httpHeaderStringMap[HttpHeader::DATE] = "date";
    httpHeaderStringMap[HttpHeader::PRAGMA] = "pragma";
    httpHeaderStringMap[HttpHeader::TRAILER] = "trailer";
    httpHeaderStringMap[HttpHeader::TRANSFER_ENCODING] = "transfer-encoding";
    httpHeaderStringMap[HttpHeader::UPGRADE] = "upgrade";
    httpHeaderStringMap[HttpHeader::VIA] = "via";
    httpHeaderStringMap[HttpHeader::WARNING] = "warning";
    httpHeaderStringMap[HttpHeader::ACCEPT] = "accept";
    httpHeaderStringMap[HttpHeader::ACCEPT_CHARSET] = "accept-charset";
    httpHeaderStringMap[HttpHeader::ACCEPT_ENCODING] = "accept-encoding";
    httpHeaderStringMap[HttpHeader::ACCEPT_LANGUAGE] = "accept-language";
    httpHeaderStringMap[HttpHeader::AUTHORIZATION] = "authorization";
    httpHeaderStringMap[HttpHeader::EXPECT] = "expect";
    httpHeaderStringMap[HttpHeader::FROM] = "from";
    httpHeaderStringMap[HttpHeader::HOST] = "host";
    httpHeaderStringMap[HttpHeader::IF_MATCH] = "if-match";
    httpHeaderStringMap[HttpHeader::IF_MODIFIED_SINCE] = "if-modified-since";
    httpHeaderStringMap[HttpHeader::IF_NONE_MATCH] = "if-none-match";
    httpHeaderStringMap[HttpHeader::IF_RANGE] = "if-range";
    httpHeaderStringMap[HttpHeader::IF_UNMODIFIED_SINCE] = "if-unmodified-since";
    httpHeaderStringMap[HttpHeader::MAX_FORWARDS] = "max-forwards";
    httpHeaderStringMap[HttpHeader::PROXY_AUTHORIZATION] = "proxy-authorization";
    httpHeaderStringMap[HttpHeader::RANGE] = "range";
    httpHeaderStringMap[HttpHeader::REFERER] = "referer";
    httpHeaderStringMap[HttpHeader::TE] = "te";
    httpHeaderStringMap[HttpHeader::USER_AGENT] = "user-agent";
    httpHeaderStringMap[HttpHeader::ACCEPT_RANGES] = "accept-ranges";
    httpHeaderStringMap[HttpHeader::AGE] = "age";
    httpHeaderStringMap[HttpHeader::ETAG] = "etag";
    httpHeaderStringMap[HttpHeader::LOCATION] = "location";
    httpHeaderStringMap[HttpHeader::PROXY_AUTHENTICATE] = "proxy-authenticate";
    httpHeaderStringMap[HttpHeader::RETRY_AFTER] = "retry-after";
    httpHeaderStringMap[HttpHeader::SERVER] = "server";
    httpHeaderStringMap[HttpHeader::VARY] = "vary";
    httpHeaderStringMap[HttpHeader::WWW_AUTHENTICATE] = "www-authenticate";
    httpHeaderStringMap[HttpHeader::ALLOW] = "allow";
    httpHeaderStringMap[HttpHeader::CONTENT_ENCODING] = "content-encoding";
    httpHeaderStringMap[HttpHeader::CONTENT_LANGUAGE] = "content-language";
    httpHeaderStringMap[HttpHeader::CONTENT_LENGTH] = "content-length";
    httpHeaderStringMap[HttpHeader::CONTENT_LOCATION] = "content-location";
    httpHeaderStringMap[HttpHeader::CONTENT_MD5] = "content-md5";
    httpHeaderStringMap[HttpHeader::CONTENT_RANGE] = "content-range";
    httpHeaderStringMap[HttpHeader::CONTENT_TYPE] = "content-type";
    httpHeaderStringMap[HttpHeader::EXPIRES] = "expires";
    httpHeaderStringMap[HttpHeader::LAST_MODIFIED] = "last-modified";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_ALLOW_ORIGIN] = "access-control-allow-origin";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_ALLOW_METHODS] = "access-control-allow-methods";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_ALLOW_HEADERS] = "access-control-allow-headers";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_MAX_AGE] = "access-control-max-age";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_ALLOW_CREDENTIALS] = "access-control-allow-credentials";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_EXPOSE_HEADERS] = "access-control-expose-headers";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_REQUEST_METHOD] = "access-control-request-method";
    httpHeaderStringMap[HttpHeader::ACCESS_CONTROL_REQUEST_HEADERS] = "access-control-request-headers";
    httpHeaderStringMap[HttpHeader::CONTENT_DISPOSITION] = "content-disposition";
    httpHeaderStringMap[HttpHeader::CONTENT_SECURITY_POLICY] = "content-security-policy";
    httpHeaderStringMap[HttpHeader::CONTENT_SECURITY_POLICY_REPORT_ONLY] = "content-security-policy-report-only";
    httpHeaderStringMap[HttpHeader::DNT] = "dnt";
    httpHeaderStringMap[HttpHeader::FORWARDED] = "forwarded";
    httpHeaderStringMap[HttpHeader::ORIGIN] = "origin";
    httpHeaderStringMap[HttpHeader::TIMING_ALLOW_ORIGIN] = "timing-allow-origin";
    httpHeaderStringMap[HttpHeader::X_CONTENT_TYPE_OPTIONS] = "x-content-type-options";
    httpHeaderStringMap[HttpHeader::X_FRAME_OPTIONS] = "x-frame-options";
    httpHeaderStringMap[HttpHeader::X_XSS_PROTECTION] = "x-xss-protection";
    httpHeaderStringMap[HttpHeader::X_FORWARDED_FOR] = "x-forwarded-for";
    httpHeaderStringMap[HttpHeader::COOKIE] = "cookie";
    httpHeaderStringMap[HttpHeader::SET_COOKIE] = "set-cookie";

    return httpHeaderStringMap;
}

// Path: srcs/constants/HttpHeaderHelper.cpp
