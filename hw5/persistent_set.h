#ifndef PERSISTENT_SET
#define PERSISTENT_SET
#include <cstdio>
#include <cstdlib>
#include <utility>
#include "linked_ptr.h"
#include "shared_ptr.h"
#include <vector>

template <typename T, template <typename> typename ptr = shared_ptr>
struct persistent_set
{
public:
    // Bidirectional iterator.
    struct iterator;

private:
    struct node
    {
        friend ptr<node>;
        friend struct persistent_set;
    private:
        ptr <T> value;
        ptr <node> left, rigth;
        node (T val):
            value(ptr<T>::make(std::move(val)))
        {
            left = ptr <node>();
            rigth = ptr <node>();
        }
        node (node const & a) noexcept:
            value(a.value),
            left(a.left),
            rigth(a.rigth)
        {}
    };
    ptr <node> left;
    void swap(persistent_set & a) noexcept
    {
        std::swap(a.left, left);
    }
    struct quard
    {
        ptr <node> pos, start, endp, pre_endp;
        iterator endi;
        quard(ptr <node> p_, ptr <node> s_, ptr <node> e_, ptr <node> p_e_, iterator ei_) noexcept:
            pos(p_),
            start(s_),
            endp(e_),
            pre_endp(p_e_),
            endi(ei_)
        {}
    };
    quard find_build(T const & a) const//pos, (start_path end_path) pos copy
    {
        ptr <node> start;
        ptr <node> endp;
        ptr <node> p_endp;
        ptr <node> tmp = left;
        iterator endi;
        endi.root = this;
        if (tmp.get() == nullptr)
            return quard(tmp, start, endp, p_endp, endi);
        start = ptr <node> :: make(*(tmp.get()));
        endp = start;
        endi.path.push_back(endp.get());
        while (*(tmp.get()->value.get()) != a)
        {
            ptr <node> endn;
            if (*(tmp.get()->value.get()) > a) //try go to left 
            {
                if (tmp.get()->left.get() == nullptr)
                    return quard(tmp, start, endp, p_endp, endi);
                tmp = tmp.get()->left;
                endn = ptr<node> :: make(*tmp.get());
                endp.get()->left = endn;
            }
            else
            {
                if (tmp.get()->rigth.get() == nullptr)
                    return quard(tmp, start, endp, p_endp, endi);
                tmp = tmp.get()->rigth;
                endn = ptr<node> :: make(*tmp.get());
                (endp.get())->rigth = endn;
            }
            p_endp = endp;
            endp = endn;
            endi.path.push_back(endp.get());
        }
        return quard(tmp, start, endp, p_endp, endi);
    }
    void merge(ptr <node> l, ptr <node> r, ptr<node> up, bool as_rigth)
    {
        if (l.get() == nullptr)
        {
            if (up.get() != nullptr)
            {
                if (as_rigth)
                    up.get()->rigth = r;
                else
                    up.get()->left = r;
            }
            else
                left = r;
            return;
        }
        if (r.get() == nullptr)
        {
            if (up.get() != nullptr)
            {
                if (as_rigth)
                    up.get()->rigth = l;
                else
                    up.get()->left = l;
            }
            else
                left = l;
            return;
        }
        if (up.get() == nullptr)
        {
            left = ptr<node> :: make(*(l.get()));
            merge(l.get()->rigth, r, left, 1);
            return;
        }
        if (as_rigth)
        {
            up.get()->rigth = ptr <node>::make(*(l.get()));
            merge(l.get()->rigth, r, up.get()->rigth, 1);
        }
        else
        {
            up.get()->left = ptr<node>::make(*(l.get()));
            merge(l.get()->rigth, r, up.get()->left, 1);
        }
    }
public:
    // Вы можете определить этот тайпдеф по вашему усмотрению.
    typedef T value_type;

    // Создает пустой persistent_set.
    persistent_set() noexcept
    {}

    // Создает копию указанного persistent_set-а.
    persistent_set(persistent_set const & a) noexcept:
        left(a.left)
    {}

    // Изменяет this так, чтобы он содержал те же элементы, что и rhs.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    persistent_set& operator=(persistent_set const & a) noexcept
    {
        left = a.left;
        return *this;
    }

    // Деструктор. Вызывается при удалении объектов persistent_set.
    // Инвалидирует все итераторы ссылающиеся на элементы этого persistent_set
    // (включая итераторы ссылающиеся на элемент следующий за послед ним).
    //~persistent_set();

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const
    {
        iterator ret;
        ret.root = const_cast <persistent_set <T, ptr> *> (this);
        node * tmp = left.get();
        while (tmp != nullptr)
        {
            ret.path.push_back(tmp);
            tmp = tmp->left.get();
        }
        return ret;
    }
    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const noexcept
    {
        iterator ret;
        ret.root = const_cast <persistent_set <T, ptr> *> (this);
        return ret;
    }

    // Поиск элемента.
    // Возвращает итератор на найденный элемент, либо end(), если элемент
    // с указанным значением отсутвует.
    iterator find(value_type a) const
    {
        ptr <node> tmp = left;
        iterator endi = end();
        if (tmp.get() == nullptr)
            return end();
        endi.path.push_back(tmp.get());
        while (*(tmp.get()->value.get()) != a)
        {
            if (*(tmp.get()->value.get()) > a) //try go to left 
            {
                if (tmp.get()->left.get() == nullptr)
                    return end();
                tmp = tmp.get()->left;
                endi.path.push_back(tmp.get());
            }
            else
            {
                if (tmp.get()->rigth.get() == nullptr)
                    return end();
                tmp = tmp.get()->rigth;
                endi.path.push_back(tmp.get());
            }
        }
        if (*(tmp.get()->value.get()) != a)
            return end();
        return endi;
    }

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Если вставка произведена, инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    std::pair<iterator, bool> insert(value_type a)
    {
        quard path = find_build(a);
        if (path.start.get() == nullptr)
        {
            left = ptr <node> :: make(std::move(a));
            return std::make_pair(begin(), 1);
        }
        left = path.start;
        if (*(path.endp.get()->value.get()) == a)
            return std::make_pair (path.endi, 0);
        if (a < *(path.endp.get()->value.get()))
        {
            path.endp.get()->left = ptr <node> :: make(std::move(a));
            return std::make_pair(path.endi--, 1); //TODO
        }
        else
        {
            path.endp.get()->rigth = ptr <node> :: make(std::move(a));
            return std::make_pair(path.endi++, 1); //TODO
        }

    }
    // Удаление элемента.
    // Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
    // 
    
    void erase(iterator a)
    {
        quard path = find_build(*(a.path.back()->value.get()));
        bool tmp = !(a.path.size() == 1 || a.path[a.path.size() - 2]->left.get() == a.path.back());
        if (path.start.get() == nullptr)
            return;
        left = path.start;
        if (path.pre_endp.get() == nullptr)
        {
            merge(path.endp.get()->left, path.endp.get()->rigth, path.pre_endp, 0);
            return;
        }
        merge(path.endp.get()->left, path.endp.get()->rigth, path.pre_endp, tmp);
    }
    

};

template <typename T, template <typename> class ptr>
struct persistent_set<T, ptr>::iterator
{
    friend struct persistent_set;
private:
    std::vector <node *> path;
    persistent_set <T, ptr> const * root;

public:
    iterator():
        root(nullptr)
    {}

    iterator (iterator const & a):
        path(a.path),
        root(a.root)
    {}
public:
        
    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end() неопределено.
    // Разыменование невалидного итератора неопределено.
    value_type const& operator*() const noexcept
    {
        return *(path.back()->value.get());
    }

    // Переход к элементу со следующим по величине ключом.
    // Инкремент итератора end() неопределен.
    // Инкремент невалидного итератора неопределен.
    iterator& operator++()
    {
        if (path.back()->rigth.get() != nullptr)
        {
            node * tmp = path.back()->rigth.get();
            path.push_back(tmp); 
            while(tmp->left.get() != nullptr)
            {
                tmp = tmp->left.get();
                path.push_back(tmp);
            }
            return *this;
        }
        else
        {
            while (1)
            {
                bool is_r = !(path.size() == 1 || path[path.size() - 2]->left.get() == path.back());
                if (!is_r)
                    break;
                path.pop_back();
            }
            path.pop_back();
            return *this;
        }
    }
    iterator operator++(int)
    {
        iterator ret = *this;
        ++(*this);
        return ret;
    }

    // Переход к элементу с предыдущим по величине ключом.
    // Декремент итератора begin() неопределен.
    // Декремент невалидного итератора неопределен.
    iterator& operator--()
    {
        if (path.empty())
        {
            node * tmp = root->left.get();
            path.push_back(tmp);
            while(tmp->rigth.get() != nullptr)
            {
                tmp = tmp->rigth.get();
                path.push_back(tmp);
            }
            return *this;
            
        }
        if ((!path.empty()) && (path.back()->left.get() != nullptr))
        {
            node * tmp = path.back()->left.get();
            path.push_back(tmp);
            while(tmp->rigth.get() != nullptr)
            {
                tmp = tmp->rigth.get();
                path.push_back(tmp);
            }
            return *this;
        }
        else
        {
            while (1)
            {
                bool is_r = !(path.size() == 1 || path[path.size() - 2]->left.get() == path.back());
                if (is_r)
                    break;
                path.pop_back();
            }
            path.pop_back();
            return *this;
        }
    }

    iterator operator--(int)
    {
        iterator ret = *this;
        --(*this);
        return ret;
    }

    // Сравнение. Итераторы считаются эквивалентными если они ссылаются на один и тот же элемент.
    // Сравнение с невалидным итератором не определено.
    // Сравнение итераторов двух разных контейнеров не определено.
    //template <typename T, template <typename> class ptr>
    friend bool operator == (iterator const & a, iterator const & b) noexcept
    {
        if (a.path.empty() && b.path.empty())
            return 1;
        if (a.path.empty() || b.path.empty())
            return 0;
        return (a.path.back() == b.path.back());
    }

    //template <typename T, template <typename> class ptr>
    friend bool operator != (iterator const & a, iterator const & b) noexcept
    {
        if (a.path.empty() && b.path.empty())
            return 0;
        if (a.path.empty() || b.path.empty())
            return 1;
        return (a.path.back() != b.path.back());
    }
};
#endif
