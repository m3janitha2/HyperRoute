#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <cstdint>
#include <string_view>
#include <examples/trading_system/protocol/ProtocolA.h>
#include <examples/trading_system/protocol/ProtocolB.h>

using namespace hyper;

/** Random number generator for a specified range of type T. */
template <typename T>
class RandomGen
{
public:
    RandomGen(T start, T end, std::uint32_t seed = 1)
        : dist_(start, end), gen_(seed) {}

    T next() noexcept { return dist_(gen_); }

private:
    std::uniform_int_distribution<T> dist_;
    std::mt19937 gen_;
};

/** TestMessageStore - Stores and generates test messages for the trading system. */
class TestMessageStore
{
public:
    TestMessageStore(std::size_t message_count)
        : uint32_generator_(1, 1000),
          uint64_generator_(1, 1000),
          int_generator_(1, 1000),
          message_selector_(0, message_count > 0 ? message_count - 1 : 0)
    {
        generate_messages(message_count);
    }

    /** Retrieve a NewOrderSingle message, setting its cl_ord_id. */
    std::string_view get_new_order(std::uint64_t cl_ord_id)
    {
        auto &msg = new_orders_[message_selector_.next()];
        msg.cl_ord_id = cl_ord_id;
        return as_string_view(msg);
    }

    /** Retrieve an ExecutionReport message, setting its field 'c' to the given cl_ord_id. */
    std::string_view get_execution_report(std::uint64_t cl_ord_id)
    {
        auto &msg = execution_reports_[message_selector_.next()];
        msg.c = cl_ord_id;
        return as_string_view(msg);
    }

    /** Retrieve a CancelReplaceRequest message, setting its orig_cl_ord_id. */
    std::string_view get_cancel_replace(std::uint64_t orig_cl_ord_id)
    {
        auto &msg = cancel_replaces_[message_selector_.next()];
        msg.orig_cl_ord_id = orig_cl_ord_id;
        return as_string_view(msg);
    }

private:
    std::vector<protocol_a::schema::NewOrderSingle> new_orders_;
    std::vector<protocol_b::schema::ExecutionReport> execution_reports_;
    std::vector<protocol_a::schema::CancelReplaceRequest> cancel_replaces_;

    RandomGen<std::uint32_t> uint32_generator_;
    RandomGen<std::uint64_t> uint64_generator_;
    RandomGen<int> int_generator_;
    RandomGen<std::size_t> message_selector_;

    void generate_messages(std::size_t message_count)
    {
        for (std::size_t i = 0; i < message_count; ++i)
        {
            new_orders_.emplace_back(protocol_a::schema::NewOrderSingle{
                .a = uint32_generator_.next(),
                .b = uint32_generator_.next(),
                .cl_ord_id = uint64_generator_.next()});

            execution_reports_.emplace_back(protocol_b::schema::ExecutionReport{
                .a = int_generator_.next(),
                .b = int_generator_.next(),
                .c = int_generator_.next()});

            cancel_replaces_.emplace_back(protocol_a::schema::CancelReplaceRequest{
                .a = uint32_generator_.next(),
                .b = uint32_generator_.next(),
                .cl_ord_id = uint64_generator_.next(),
                .orig_cl_ord_id = uint64_generator_.next()});
        }
    }

    template <typename T>
    std::string_view as_string_view(T &msg)
    {
        return std::string_view(reinterpret_cast<char *>(&msg), msg.header.size);
    }
};
