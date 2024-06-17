#include <cassert>
#include <sol/sol.hpp>

#include <include/core/SkM44.h>
#include <string>

using namespace std;
class Base
{
public:
    Base(int i, const std::string &str)
        : m_data(i)
        , m_des(str)
    {}

    int data() { return m_data; }
    void set_data(int i) { m_data = i; }

    const std::string &des() { return m_des; }
    void set_des(const std::string &str) { m_des = str; }

    const std::string &name() const { return m_class_name; }

    const SkV3 &d3() const { return m_d3; }
    void set_d3(const SkV3 &d3) { m_d3 = d3; }

private:
    int m_data;
    std::string m_des;
    std::string m_class_name = "Base";
    SkV3 m_d3;
};

// Small example class  similar to magnum vector2
template<std::size_t size, class T>
class MyVector
{
protected:
    T _data[size];
};

template<typename T>
class MyVector2 : public MyVector<2, T>
{
public:
    T &x() { return MyVector<2, T>::_data[0]; }
    const T &x() const { return MyVector<2, T>::_data[0]; }
    T &y() { return MyVector<2, T>::_data[1]; }
    const T &y() const { return MyVector<2, T>::_data[1]; }
};

int main()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package);
    lua.script("print('bark bark bark!')");

    using MyVec2D = MyVector2<float>;
    MyVec2D my_vec;

    // Gives compilation error
    sol::usertype<MyVec2D> my_vec2_type = lua.new_usertype<MyVec2D>("MyVec2D",
                                                                    sol::constructors<MyVec2D()>());
    my_vec2_type.set("x",
                     sol::property([](MyVec2D &type) { return type.x(); },
                                   [](MyVec2D &type, float val) { type.x() = val; }));
    my_vec2_type.set("y",
                     sol::property([](MyVec2D &type) { return type.y(); },
                                   [](MyVec2D &type, float val) { type.y() = val; }));

    MyVec2D r_vec = lua.script("vec2 = MyVec2D.new()\n"
                               "vec2.x = 5\n"
                               "vec2.y = 7\n"
                               "return vec2");
    cout << "Vec2D : " << r_vec.x() << " " << r_vec.y() << endl;

    // make usertype metatable
    sol::usertype<Base> base_type
        = lua.new_usertype<Base>("Base",
                                 // 3 constructors
                                 sol::constructors<Base(int, const std::string &)>());
    sol::usertype<SkV3> vec3_typ = lua.new_usertype<SkV3>("Vec3D",
                                                          // 3 constructors
                                                          sol::constructors<SkV3()>());
    vec3_typ["x"] = &SkV3::x;
    vec3_typ["y"] = &SkV3::y;
    vec3_typ["z"] = &SkV3::z;

    sol::usertype<SkV2> vec2_typ = lua.new_usertype<SkV2>("Vec2D");
    vec2_typ["x"] = &SkV2::x;
    vec2_typ["y"] = &SkV2::y;

    base_type["data"] = sol::property(&Base::data, &Base::set_data);
    base_type["set_des"] = &Base::set_des;
    base_type["des"] = &Base::des;
    base_type["d3"] = &Base::d3;
    //    base_type["m_d3"] = &Base::m_d3;  // This does not work
    base_type.set("name", sol::readonly(&Base::name));

    //    base_type["description"] = &Base::des;
    lua.script("base = Base.new(1, \"Hello\")\n"
               "base.data = 12\n"
               "b_name = base:name()");

    Base &base = lua["base"];
    const std::string &name = lua["b_name"];
    cout << base.data() << " " << name << endl;

    /////////////////////////////////////////
    Base b2(43, "asdasd");
    lua["base2"] = std::ref(b2);
    lua.script("base2:set_des(\"BASE 2\")");
    cout << b2.des() << endl;

    SkV3 ret = lua.script("return base2:d3()");
    cout << ret.x << " " << ret.y << " " << ret.z << endl;
    b2.set_d3({123.1, 123.1, 123});
    ret = lua.script("return base2:d3()");
    cout << ret.x << " " << ret.y << " " << ret.z << endl;
    ret = lua.script("vec3 = Vec3D.new()\n"
                     "vec3.x = 0\n"
                     "vec3.y = 1\n"
                     "vec3.z = 2\n"
                     "return vec3");
    cout << ret.x << " " << ret.y << " " << ret.z << endl;

    SkV2 vec{123.23, 532.123};
    lua["obj2d"] = std::ref(vec);
    float x = lua.script("return obj2d.x");
    float y = lua.script("return obj2d.y");
    float result = lua.script("return (obj2d.y + obj2d.x)*0.5");
    cout << x << " " << y << " average = " << result << endl;

    /////////////////////////////////////////
    const std::string &name2 = lua.script("base2:set_des(\"CHECK\")\n"
                                          "return base2:des()");
    cout << name2 << endl;
    return 0;
}
