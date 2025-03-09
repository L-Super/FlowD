//
// Created by Leo on 24-8-26.
//

#pragma once

namespace noncopyable_ {
class noncopyable {
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};
}// namespace noncopyable_

using Noncopyable = noncopyable_::noncopyable;
