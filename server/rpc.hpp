//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/BeastLounge
//

#ifndef LOUNGE_RPC_HPP
#define LOUNGE_RPC_HPP

#include "config.hpp"
#include <boost/beast/core/string.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/optional.hpp>
#include <stdexcept>

/// Codes used in JSON-RPC error responses
enum class rpc_code
{
    parse_error      = -32700,
    invalid_request  = -32600,
    method_not_found = -32601,
    invalid_params   = -32602,
    internal_error   = -32603,

    /// Expected object in JSON-RPC request
    expected_object = 1,

    /// Expected string version in JSON-RPC request
    expected_string_version,

    /// Uknown version in JSON-RPC request
    unknown_version,

    /// Invalid null id in JSON-RPC request
    invalid_null_id,

    /// Expected string or number id in JSON-RPC request
    expected_strnum_id,

    /// Missing id in JSON-RPC request version 1
    expected_id,

    /// Missing method in JSON-RPC request
    missing_method,

    /// Expected string method in JSON-RPC request
    expected_string_method,

    /// Expected structured params in JSON-RPC request version 2
    expected_structured_params,

    /// Missing params in JSON-RPC request version 1
    missing_params,

    /// Expected array params in JSON-RPC request version 1
    expected_array_params
};

namespace boost {
namespace system {
template<>
struct is_error_code_enum<rpc_code>
{
    static bool constexpr value = true;
};
} // system
} // boost

beast::error_code
make_error_code(rpc_code e);

//------------------------------------------------------------------------------

class rpc_error
    : public std::exception
{
    int code_;
    std::string msg_;

public:
    rpc_error()
        : rpc_error(
            rpc_code::internal_error)
    {
    }

    rpc_error(
        rpc_code ev)
        : rpc_error(ev, 
            beast::error_code(ev).message())
    {
    }

    rpc_error(
        beast::string_view msg)
        : rpc_error(
            rpc_code::invalid_params,
            msg)
    {
    }

    rpc_error(
        rpc_code ev,
        beast::string_view msg)
        : code_(static_cast<int>(ev))
        , msg_(msg)
    {
    }

    rpc_error(
        beast::error_code const& ec)
        : code_(static_cast<int>(ec.value()))
        , msg_(ec.message())
    {
    }

    json::value
    to_json(
        boost::optional<json::value> const&
            id = boost::none) const;
};

//------------------------------------------------------------------------------

/** Represents a JSON-RPC request
*/
class rpc_request
{
public:
    /// Version of the request (1 or 2)
    int version = 2;

    /// The request method
    json::string method;

    /** The request parameters

        This will be object, array, or null
    */
    json::value params;

    /** The request id

        If set, this will be string, number, or null
    */
    boost::optional<json::value> id;

    /** Construct an empty request using the default storage.

        The method, params, and id will be null,
        and version will be 2.
    */
    rpc_request() = default;

    /** Construct an empty request using the specified storage.

        The method, params, and id will be null,
        and version will be 2.
    */
    explicit
    rpc_request(json::storage_ptr sp);

    /** Extract a JSON-RPC request or return an error.
    */
    void
    extract(
        json::value&& jv,
        beast::error_code& ec);
};

//------------------------------------------------------------------------------

extern
json::value
make_rpc_error(
    rpc_code ev,
    beast::string_view msg);

extern
json::value
make_rpc_error(
    rpc_code ev,
    beast::string_view msg,
    rpc_request& req);

extern
json::object&
checked_object(json::value& jv);

extern
json::array&
checked_array(json::value& jv);

extern
json::string&
checked_string(json::value& jv);

extern
json::number&
checked_number(json::value& jv);

extern
bool&
checked_bool(json::value& jv);

extern
void
checked_null(json::value& jv);

extern
json::value&
checked_value(
    json::value& jv,
    beast::string_view key);

extern
json::object&
checked_object(
    json::value& jv,
    beast::string_view key);

extern
json::array&
checked_array(
    json::value& jv,
    beast::string_view key);

extern
json::string&
checked_string(
    json::value& jv,
    beast::string_view key);

extern
json::number&
checked_number(
    json::value& jv,
    beast::string_view key);

extern
bool&
checked_bool(
    json::value& jv,
    beast::string_view key);

extern
void
checked_null(
    json::value& jv,
    beast::string_view key);

#endif
