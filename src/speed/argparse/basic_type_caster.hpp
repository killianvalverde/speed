/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file        speed/argparse/basic_type_caster.hpp
 * @brief       basic_type_caster main header.
 * @author      Killian Valverde
 * @date        2024/05/15
 */

#ifndef SPEED_ARGPARSE_BASIC_TYPE_CASTER_HPP
#define SPEED_ARGPARSE_BASIC_TYPE_CASTER_HPP

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../type_casting/type_casting.hpp"


namespace speed::argparse {


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_type_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    basic_type_caster(target_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (!speed::type_casting::try_type_cast(arg, holdr_, err_code))
        {
            return component_type::try_type_cast(arg, holdr_, err_code);
        }

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    target_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        std::size_t Nm,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_array_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Vector type used in the class. */
    using array_type = std::array<target_type, Nm>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_array_caster(array_type* holdr)
            : holdr_(holdr)
            , idx_(0)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        auto& res = holdr_->at(idx_);

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        ++idx_;

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    array_type* holdr_;

    /** Index of the next element in which set the casting result. */
    std::size_t idx_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_vector_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Vector type used in the class. */
    using vector_type = std::vector<target_type, allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_vector_caster(vector_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace_back(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    vector_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_deque_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Vector type used in the class. */
    using deque_type = std::deque<target_type, allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_deque_caster(deque_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace_back(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    deque_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_queue_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Vector type used in the class. */
    using deque_type = std::deque<target_type, allocator_type<target_type>>;

    /** Queue type used in the class. */
    using queue_type = std::queue<target_type, deque_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_queue_caster(deque_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    queue_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_priority_queue_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Vector type used in the class. */
    using vector_type = std::vector<target_type, allocator_type<target_type>>;

    /** Priority queue type used in the class. */
    using priority_queue_type = std::priority_queue<target_type, vector_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_priority_queue_caster(priority_queue_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    priority_queue_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_stack_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Double ended queue type used in the class. */
    using deque_type = std::deque<target_type, allocator_type<target_type>>;

    /** Stack type used in the class. */
    using stack_type = std::stack<target_type, deque_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_stack_caster(stack_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    stack_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_forward_list_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Forward list type used in the class. */
    using forward_list_type = std::forward_list<target_type, allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_forward_list_caster(forward_list_type* holdr)
            : holdr_(holdr)
            , it_(holdr_->before_begin())
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        it_ = holdr_->emplace_after(it_, std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    forward_list_type* holdr_;

    /** Iterator that holds the next position in which insert the element. */
    typename forward_list_type::iterator it_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_list_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Forward list type used in the class. */
    using list_type = std::list<target_type, allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_list_caster(list_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace_back(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    list_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_set_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Set type used in the class. */
    using set_type = std::set<target_type, std::less<target_type>, allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_set_caster(set_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    set_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_unordered_set_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Unordered set type used in the class. */
    using unordered_set_type = std::unordered_set<
            target_type, std::hash<target_type>, std::equal_to<target_type>,
            allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_unordered_set_caster(unordered_set_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    unordered_set_type* holdr_;
};


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        typename TpAllocator,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_unordered_multiset_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Unordered set type used in the class. */
    using unordered_multiset_type = std::unordered_multiset<
            target_type, std::hash<target_type>, std::equal_to<target_type>,
            allocator_type<target_type>>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_unordered_multiset_caster(unordered_multiset_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type res;

        if (!speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::try_type_cast(arg, &res, err_code);
        }

        holdr_->emplace(std::move(res));

        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    unordered_multiset_type* holdr_;
};


}


#endif
