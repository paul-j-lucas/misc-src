// standard
#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace PJL {

/**
 * Implementation traits type for mbr_map.
 */
template<class ClassType>
struct mbr_map_traits {
    typedef std::string mbr_name_type;
    typedef void* ClassType::*any_mbr_ptr_type;
    typedef std::map<mbr_name_type,any_mbr_ptr_type> map_type;
};

template<class ClassType>
struct mbr_map : mbr_map_traits<ClassType>::map_type {
    typedef mbr_map_traits<ClassType> traits;
    typedef typename traits::mbr_name_type mbr_name_type;
    typedef typename traits::any_mbr_ptr_type any_mbr_ptr_type;

    template<typename MemberType>
    void mbr_init( mbr_name_type const &name, MemberType ClassType::*ptr ) {
        (*this)[ name ] = reinterpret_cast<any_mbr_ptr_type>( ptr );
    }

    template<typename MemberType>
    bool mbr_set( ClassType &c, mbr_name_type const &name,
                  MemberType const &value ) {
        typedef typename mbr_map<ClassType>::const_iterator const_iterator;
        const_iterator const found = this->find( name );
        if ( found != this->end() ) {
            typedef MemberType ClassType::*mbr_ptr_type;
            c.*reinterpret_cast<mbr_ptr_type>( found->second ) = value;
            return true;
        }
        return false;
    }
};

} // namespace PJL

///////////////////////////////////////////////////////////////////////////////

#include <iostream>

struct S {
    std::string s;
    int i;
    bool b;
};

void mbr_map_init( PJL::mbr_map<S> *m ) {
    m->mbr_init( "string_mbr", &S::s );
    m->mbr_init( "int_mbr", &S::i );
    m->mbr_init( "bool_mbr", &S::b );
}

using namespace std;

int main() {
    PJL::mbr_map<S> m;
    mbr_map_init( &m );

    S s;

    m.mbr_set( s, "string_mbr", string( "hello" ) );
    m.mbr_set( s, "int_mbr", 42 );
    m.mbr_set( s, "bool_mbr", true );

    cout << s.s << endl;
    cout << s.i << endl;
    cout << s.b << endl;
}

///////////////////////////////////////////////////////////////////////////////
/* vim:set et sw=4 ts=4: */
