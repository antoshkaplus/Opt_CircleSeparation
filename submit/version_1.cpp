

#include <vector>
#include <tuple>
#include <map>
#include <numeric>
#include <random>
#include <set>
#include <cassert>

namespace ant {
    
    using Int = int;
    // index and count should be of type int
    // because 
    // 1) unsigned types increase probability of making a bug
    // 2) lesser type will create problem of casting or being too little
    // 3) bigger type impossible to iterate through
    // the only thing is unsigned integers is good for bitwise operations
    using Count = int; 
    using Index = int;
    
    using Long = int64_t;
    using Float = float;
    using Double = double;
}


namespace ant {
    
    namespace geometry {
        
        namespace d2 {
            
            
            
            template<class T>
            struct Size {
                Size() : Size(0, 0) {}
                Size(T width, T height)
                : width(width), height(height) {}
                
                void set(T width, T height) {
                    this->width = width;
                    this->height = height;
                }
                T area() const {
                    return width*height;
                }
                T perimeter() const {
                    return 2*(height + width); 
                }
                bool isCovering(const Size<T>& s) const {
                    return width >= s.width && height >= s.height;
                }
                void swap() {
                    std::swap(height, width);
                }
                
                Size swapped() const {
                    return Size(height, width);
                }
                
                T width, height;
            };
            
            template<class T>
            bool operator==(const Size<T>& s_0, const Size<T>& s_1) {
                return s_0.width == s_1.width && s_0.height == s_1.height;
            }
            template<class T>
            bool operator!=(const Size<T>& s_0, const Size<T>& s_1) {
                return s_0.width != s_1.width || s_0.height != s_1.height;
            }
            
            
            namespace i {
                
                using Size = d2::Size<size_t>;
                
                struct Point {
                    Point() {}
                    Point(Int x, Int y) : x(x), y(y) {}
                    void set(Int x, Int y) {
                        this->x = x;
                        this->y = y;
                    }
                    void swap() {
                        std::swap(x, y);
                    }
                    Int x, y;
                };
                
                struct Segment {
                    Segment() {}
                    Segment(const Point& fst, const Point& snd)
                    : fst(fst), snd(snd) {}
                    Point fst, snd;
                    
                    void swap() {
                        std::swap(fst, snd);
                    }
                    
                    Segment swapped() const {
                        return Segment(snd, fst);
                    }
                };
                
                bool operator==(const Point& p_0, const Point& p_1);
                bool operator!=(const Point& p_0, const Point& p_1);
                
                struct Rectangle {
                    Rectangle() {}
                    Rectangle(const Point& origin, const Size& size) 
                    : origin(origin), size(size) {}
                    Rectangle(Int x, Int y, Int width, Int height) 
                    : origin(x, y), size(width, height) {}
                    
                    void set(Int x, Int y, size_t width, size_t height) {
                        origin.set(x, y);
                        size.set(width, height);
                    }
                    
                    void set(const Point& origin, const Point& diag) {
                        this->origin = origin;
                        size.set(diag.x - origin.x, diag.y - origin.y);
                    }
                    
                    void swap() {
                        origin.swap();
                        size.swap();
                    }
                    
                    size_t area() const {
                        return size.area();
                    }
                    
                    size_t perimeter() const {
                        return size.perimeter(); 
                    }
                    
                    bool isIntersect(const Rectangle& r) const {
                        return origin.x < r.origin.x + r.size.width  && origin.x + size.width  > r.origin.x &&
                        origin.y < r.origin.y + r.size.height && origin.y + size.height > r.origin.y;
                    }
                    
                    Point origin;
                    Size size;
                };
                
                bool operator==(const Rectangle& r_0, const Rectangle& r_1);
                bool operator!=(const Rectangle& r_0, const Rectangle& r_1);    
                
                // oab
                // Returns a positive value, if p_0, p_1, p_2 makes a counter-clockwise turn,
                // negative for clockwise turn, and zero if the points are collinear.
                static int cross_product(const Point& p_0, const Point& p_1, const Point& p_2) {
                    return (p_1.x - p_0.x)*(p_2.y - p_0.y) - (p_1.y - p_0.y)*(p_2.x - p_0.x);
                }
                
                std::vector<Index> convex_hull(const std::vector<Point>& ps, bool is_clockwise);    
                
            } // namespace i
            
            
            namespace f {
                
                
                struct Point {
                    Point() : Point(0, 0) {}
                    Point(Float x, Float y) : x(x), y(y) {}
                    
                    Float distance(const Point& p) const {
                        Float 
                        dx = p.x - x,
                        dy = p.y - y;
                        return sqrt(dx*dx + dy*dy);
                    } 
                    
                    static Float distance(const Point& p_0, const Point& p_1) {
                        return p_0.distance(p_1);
                    }
                    
                    void set(Float x, Float y) {
                        this->x = x;
                        this->y = y;
                    }
                    
                    Float x, y;
                };
                
                Point& operator+=(Point& p_0, const Point& p_1);    
                Point& operator/=(Point& p_0, Float f);
                Point operator+(Point p_0, const Point& p_1);
                Point operator/(Point p_0, Float f);
                
                
                struct Indent {
                    Indent() : Indent(0, 0) {}
                    Indent(Float dx, Float dy) : dx(dx), dy(dy) {}
                    
                    Indent& operator+=(const Indent& d) {
                        dx += d.dx;
                        dy += d.dy;
                        return *this;
                    }
                    
                    Float distance() const {
                        return sqrt(dx*dx + dy*dy);
                    }
                    
                    Indent normed() const {
                        auto d = distance();
                        return {dx/d, dy/d};
                    }
                    
                    Float dx, dy;
                };
                
                Indent& operator/=(Indent& i, Float f);
                Indent& operator*=(Indent& i, Float f); 
                Indent operator/(Indent i, Float f);
                Indent operator*(Indent i, Float f);
                Indent operator+(Indent i_0, Indent i_1);
                Point& operator+=(Point& p, Indent i);
                Indent operator-(const Point& p_0, const Point& p_1); 
                Indent operator*(Float d, Indent i);
                
                
                using Size = d2::Size<double>;
                
                struct Line {
                    Line() : Line(0, 0, 0) {}
                    Line(double a, double b, double c) : a(a), b(b), c(c) {}
                    Line(const Point& p_0, const Point& p_1) {
                        a = p_1.y - p_0.y;
                        b = p_0.x - p_1.x;
                        c = p_0.x*(p_0.y - p_1.y) + p_0.y*(p_1.x - p_0.x);
                    }
                    
                    double a, b, c;
                };
                
                struct Circle {
                    Circle() : radius(0) {}
                    Circle(Point center, double radius) : center(center), radius(radius) {}
                    Point center;
                    double radius;
                };
                
                struct Rectangle {
                    Rectangle() : origin(0, 0), size(0, 0) {}
                    Rectangle(Float x, Float y, Float width, Float height) 
                    : origin(x, y), size(width, height) {}
                    Rectangle(Point origin, Size size) 
                    : origin(origin), size(size) {}
                    
                    bool isInside(const Point& p) const {
                        return p.x >= origin.x && p.y >= origin.y && 
                        p.x <= origin.x+size.width && p.y <= origin.y+size.height;
                    }
                    
                    Point origin;
                    Size size; 
                };
                
                bool operator==(const Point& p_0, const Point& p_1);
                std::ostream& operator<<(std::ostream& output, const Point& p);
                std::pair<Point, Point> circleLineIntersection(const Circle& circle, const Line& line);
                
            } // namespace f
            
            
            
        } // namespace d2
        
    } // namespace geometry
    
}


namespace ant {
    
    namespace geometry {
        
        namespace d2 {
            
            namespace f {
                
                
                Point& operator+=(Point& p_0, const Point& p_1) {
                    p_0.x += p_1.x;
                    p_0.y += p_1.y;
                    return p_0;
                }
                
                Point& operator/=(Point& p_0, Float f) {
                    p_0.x /= f;
                    p_0.y /= f;
                    return p_0;
                } 
                
                Point operator+(Point p_0, const Point& p_1) {
                    return p_0 += p_1;
                }
                
                Point operator/(Point p_0, Float f) {
                    return p_0 /= f;
                }
                
                Indent& operator/=(Indent& i, Float f) {
                    i.dx /= f;
                    i.dy /= f;
                    return i;
                }
                
                Indent& operator*=(Indent& i, Float f) {
                    i.dx *= f;
                    i.dy *= f;
                    return i;
                }
                
                Indent operator/(Indent i, Float f) {
                    i /= f;
                    return i;
                }
                
                Indent operator*(Indent i, Float f) {
                    i *= f;
                    return i;
                }
                
                Indent operator+(Indent i_0, Indent i_1) {
                    i_0.dx += i_1.dx;
                    i_0.dy += i_1.dy;
                    return i_0;
                }
                
                Point& operator+=(Point& p, Indent i) {
                    p.x += i.dx;
                    p.y += i.dy;
                    return p;
                }
                
                Indent operator-(const Point& p_0, const Point& p_1) {
                    Indent i;
                    i.dx = p_0.x - p_1.x;
                    i.dy = p_0.y - p_1.y;
                    return i;
                }
                
                Indent operator*(Float d, Indent i) {
                    i *= d;
                    return i;
                }
                
                
            }
            
        }
        
    }
    
}




namespace ant {
    
    namespace grid {
        
        
        // make it possible to substruct
        struct Indent {
            Indent() : Indent(0, 0) {}
            Indent(Int row, Int col) : row(row), col(col) {}
            
            void set(Int row, Int col) {
                this->row = row;
                this->col = col;
            }
            
            Int area() const {
                return row*col;
            }
            
            Int row, col;
        };
        
        struct Size {
            Size() : row(0), col(0) {}
            Size(Int row, Int col)
            : row(row), col(col) {}
            
            void set(Int row, Int col) {
                this->row = row;
                this->col = col;
            }
            
            Count cell_count() const {
                return row*col;
            }
            
            Size swapped() const {
                return Size(col, row);
            }
            
            void swap() {
                std::swap(row, col);
            }
            
            Int row, col;
        };
        
        bool operator==(const Size& s_0, const Size& s_1);
        
        // don't really know what to do with it
        //template<class T>
        //struct Position {
        //    using value_type = T;
        //        
        //    T row, col;
        //    
        //    Position() {}
        //    Position(T row, T col) : row(row), col(col) {}
        //    
        //    void set(Int row, Int col) {
        //        this->row = row;
        //        this->col = col;
        //    }
        //    
        //    void shift(Int row, Int col) {
        //        this->row += row;
        //        this->col += col;
        //    }
        //    
        //    void swap() {
        //        std::swap(row, col);
        //    }
        //    Position swapped() const {
        //        return Position(col, row);
        //    }
        //    
        //    Position shifted(Int row_shift, Int col_shift) const {
        //        return {row + row_shift, col + col_shift};
        //    }
        //    
        //    struct TopLeftComparator {
        //        bool operator()(const Position& p_0, const Position& p_1) {
        //            return p_0.row < p_1.row || (p_0.row == p_1.row && p_0.col < p_1.col);
        //        }
        //    };
        //    struct BottomRightComparator {
        //        bool operator()(const Position& p_0, const Position& p_1) {
        //            return p_0.row > p_1.row || (p_0.row == p_1.row && p_0.col > p_1.col);
        //        }
        //    };
        //};
        
        
        
        struct Position {
            // operators see below
            constexpr Position() : Position(row, col) {}
            constexpr Position(Int row, Int col) : row(row), col(col) {}
            
            void set(Int row, Int col) {
                this->row = row;
                this->col = col;
            }
            
            void shift(Int row, Int col) {
                this->row += row;
                this->col += col;
            }
            
            void swap() {
                std::swap(row, col);
            }
            Position swapped() const {
                return Position(col, row);
            }
            
            Position shifted(Int row_shift, Int col_shift) const {
                return {row + row_shift, col + col_shift};
            }
            
            Int row, col;
            
            struct TopLeftComparator {
                bool operator()(const Position& p_0, const Position& p_1) {
                    return p_0.row < p_1.row || (p_0.row == p_1.row && p_0.col < p_1.col);
                }
            };
            struct BottomRightComparator {
                bool operator()(const Position& p_0, const Position& p_1) {
                    return p_0.row > p_1.row || (p_0.row == p_1.row && p_0.col > p_1.col);
                }
            };
        };   
        
        
        Position operator-(const Position& p, const Indent& n);
        Position operator+(const Position& p, const Indent& n);        
        bool operator==(const Position& p_0, const Position& p_1);        
        bool operator!=(const Position& p_0, const Position& p_1);
        
        struct Region {
            Region() : Region(0, 0, 0, 0) {}
            Region(Int row, Int col, Int row_count, Int col_count)
            : position(row, col), size(row_count, col_count) {}
            Region(const Position& p, const Size& d) 
            : position(p), size(d) {}
            
            void set(Int row, Int col, Int row_indent, Int col_indent) {
                position.set(row, col);
                size.set(row_indent, col_indent);
            }
            void set(const Position& p, const Size& d) {
                position = p;
                size = d;
            }
            
            void shift(Int row, Int col) {
                position.row += row;
                position.col += col;
            } 
            
            // maybe someone will find those getters useful 
            Int row_begin() const { return position.row; }
            Int row_end()   const { return position.row + size.row; }
            Int col_begin() const { return position.col; }
            Int col_end()   const { return position.col + size.col; }
            
            Int col_count() const { return size.col; }
            Int row_count() const { return size.row; }
            
            void set_row_begin(Int r) { 
                size.row += position.row - r; 
                position.row = r; 
            }
            void set_row_end(Int r) { 
                size.row = r - position.row; 
            }
            void set_col_begin(Int c) { 
                size.col += position.col - c;
                position.col = c; 
            }
            void set_col_end(Int c) { 
                size.col = c - position.col; 
            }
            
            bool hasInside(const Position& p) const {
                return p.row >= position.row && 
                p.row  < position.row+size.row &&
                p.col >= position.col &&
                p.col  < position.col+size.col;
            }
            bool hasInside(const Region& region) const {
                return row_begin() <= region.row_begin() 
                && col_begin() <= region.col_begin()
                && row_end() >= region.row_end()
                && col_end() >= region.col_end();
            }
            
            Region intersection(const Region& r) const {
                Region n;
                n.position.row = std::max(position.row, r.position.row);
                n.position.col = std::max(position.col, r.position.col);
                n.size.row = std::min(row_end(), r.row_end()) - n.position.row;
                n.size.col = std::min(col_end(), r.col_end()) - n.position.col;
                
                if (n.size.row < 0 || n.size.col < 0) {
                    n.size.row = 0;
                    n.size.col = 0;
                }
                return n;
            }
            
            bool hasIntersection(const Region& r) const {
                return !intersection(r).isEmpty();
            }
            
            bool isEmpty() const { return size.row == 0 || size.col == 0; }
            
            template<class ForwardIterator>
            static Region unite(ForwardIterator first, ForwardIterator last) {
                Region rect = *first;
                while (++first != last) {
                    // should update "end" first because it dependent on "begin"
                    rect.set_row_end(std::max(first->row_end(), rect.row_end()));
                    rect.set_col_end(std::max(first->col_end(), rect.col_end()));
                    rect.set_row_begin(std::min(first->row_begin(), rect.row_begin()));
                    rect.set_col_begin(std::min(first->col_begin(), rect.col_begin()));
                    
                }
                return rect;
            }
            
            void Unite(const Region& r) {
                
            }
            
            Region United(const Region& r) {
                return Region();
            }
            
            Int cell_count() const {
                return size.row*size.col;
            }
            
            Position position;
            Size size;
        };
        
        std::ostream& operator<<(std::ostream& output, const Region& r);
        bool operator==(const Region& r_0, const Region& r_1);
        
        
        
        // G - grid
        template<class G>
        struct GridView {
            
            using value_type = typename G::value_type;
            using const_reference = const value_type&;
        private:
            using real_type = typename std::conditional<std::is_const<G>::value, const value_type, value_type>::type;
        public:
            
            
            GridView(std::shared_ptr<G> grid, const Position& origin) : grid_(grid), origin_(origin) {}
            GridView() : origin_(0, 0) {}
            
            const_reference operator()(const Position& p) const {
                return (*grid_)(p + origin_);
            }
            const_reference operator()(Index row, Index col) const {
                return (*grid_)(row + origin_.row, col + origin_.col);
            }
            
            
            real_type operator()(const Position& p) {
                return (*grid_)(p + origin_);
            }
            real_type operator()(Index row, Index col) {
                return (*grid_)(row + origin_.row, col + origin_.col);
            }
            
            
            void set_origin(const Position& p) {
                origin_ = p;
            }
            
            const Size& size() const {
                return grid_->size();
            }
            
        private:
            
            
            std::shared_ptr<G> grid_;
            Position origin_;
        };
        
        template<class G>
        class OriginGrid {
        public:
            using value_type = typename G::value_type;
            
            OriginGrid() {}
            OriginGrid(Position origin, Size size) : origin_(origin), grid_(size) {}
            
            
            value_type& operator()(const Position& p) {
                return grid_(p.row - origin_.row, p.col - origin_.col);
            }
            const value_type& operator()(const Position& p) const {
                return grid_(p.row - origin_.row, p.col - origin_.col);
            }
            
            value_type& operator[](const Position& p) {
                return grid_(p.row - origin_.row, p.col - origin_.col);
            }
            const value_type& operator[](const Position& p) const {
                return grid_(p.row - origin_.row, p.col - origin_.col);
            }
            
            value_type& operator()(Int row, Int col) {
                return grid_(row - origin_.row, col - origin_.col);
            }
            const value_type& operator()(Int row, Int col) const {
                return grid_(row - origin_.row, col - origin_.col);
            }
            
            
            G& grid() {
                return grid_;
            }
            
            const G& grid() const {
                return grid_;
            }
            
            void set_grid(G&& grid) {
                grid_ = std::move(grid);
            } 
            
            void set_grid(const G& grid) {
                grid_ = grid;
            }
            
            void set_origin(const Position& p) {
                origin_ = p;
            }
            
            Position& origin() {
                return origin_;
            }
            const Position origin() const {
                return origin_;
            }
            
        private:
            Position origin_;
            G grid_;
        };
        
        template<class T>
        struct Grid {
            using value_type = T;
            
            typename std::vector<T>::const_iterator begin() const {
                return grid_.begin();
            }
            typename std::vector<T>::iterator begin() {
                return grid_.begin();
            }
            typename std::vector<T>::const_iterator end() const {
                return grid_.end();
            }
            typename std::vector<T>::iterator end() {
                return grid_.end();
            }
            
            
            Grid() : Grid(0, 0) {}
            Grid(Count row_count, Count col_count)
            :   row_count_(row_count), 
            col_count_(col_count), 
            grid_(row_count*col_count) {}
            
            Grid(Count row_count, Count col_count, const T& value_type) 
            :   row_count_(row_count),
            col_count_(col_count),
            grid_(row_count*col_count, value_type) {}
            
            Grid(const Size& size) : Grid(size.row, size.col) {}
            Grid(std::initializer_list<std::vector<T>> list)
            :   Grid(list.size(), list.size() == 0 ? 0 : list.begin()->size()){
                auto b = list.begin();
                for (auto r = 0; r < row_count(); ++r, ++b) {
                    std::copy(b->begin(), b->end(), grid_.begin() + r*col_count());
                }
            }
            
            bool isInside(const Position& p) {
                return isInside(p.row, p.col);
            }
            bool isInside(Int row, Int col) {
                return row >= 0 && row < row_count_ && 
                col >= 0 && col < col_count_;
            }
            
            void resize(Count row_count, Count col_count) {
                row_count_ = row_count;
                col_count_ = col_count;
                grid_.resize(row_count*col_count);
            }
            void resize(const Size& size) {
                resize(size.row, size.col);
            }
            
            void fill(const T& t) {
                std::fill(grid_.begin(), grid_.end(), t);
            }
            
            Count row_count() const { return row_count_; }
            Count col_count() const { return col_count_; }
            Size size() const { return Size(row_count(), col_count()); } 
            Count cell_count() const { return row_count()*col_count(); } 
            
            T& operator()(const Position& p) {
                return grid_[p.row*col_count_ + p.col];
            }
            const T& operator()(const Position& p) const {
                return grid_[p.row*col_count_ + p.col];
            }
            
            T& operator[](const Position& p) {
                return grid_[p.row*col_count_ + p.col];
            }
            const T& operator[](const Position& p) const {
                return grid_[p.row*col_count_ + p.col];
            }
            
            T& operator()(Int row, Int col) {
                return grid_[row*col_count_ + col];
            }
            const T& operator()(Int row, Int col) const {
                return grid_[row*col_count_ + col];
            }
        private:
            Count row_count_, col_count_;
            std::vector<T> grid_;
            
            friend struct const_iterator;
            template<class U>
            friend bool operator==(const Grid<U>& g_0, const Grid<U>& g_1);
        };

        // we think that top left is start  
        struct ParticleGrid {
            using Point = ant::geometry::d2::f::Point;
            using indent = ant::geometry::d2::f::Indent;
            
            // particles will be compared by address
            struct Particle {
                virtual bool intersects(const Particle& p) const = 0;
                // maybe to use position (but it can be confused with Position and left top angle)
                virtual Point center() const = 0;
            };
            
            ParticleGrid() {}
            ParticleGrid(
                         Float x_min, Float y_min, 
                         Float x_max, Float y_max,
                         Float max_particle_width, 
                         Float max_particle_height) {
                
                reset(x_min, y_min, x_max, y_max, max_particle_width, max_particle_height);
            }
            
            void clear() {
                for (auto row = 0; row < grid.row_count(); ++row) {
                    for (auto col = 0; col < grid.col_count(); ++col) {
                        grid(row, col).clear();
                    }
                }
            }
            
            void reset(Float x_min, Float y_min, 
                       Float x_max, Float y_max,
                       Float max_particle_width, 
                       Float max_particle_height) {
                clear();
                
                this->x_min = x_min;
                this->x_max = x_max;
                this->y_min = y_min;
                this->y_max = y_max;
                
                side_width = max_particle_width; // maybe need to add + some offset
                side_height = max_particle_height;
                
                Count 
                row_count = static_cast<Count>((x_max-x_min)/max_particle_width) + 1, 
                col_count = static_cast<Count>((y_max-y_min)/max_particle_height) + 1;
                grid.resize(row_count, col_count);
            }
            
            void add(const std::shared_ptr<Particle>& p) {
                grid(position(*p)).push_back(p);
            }
            
            // before you are going to change location of particle remove it from grid
            bool remove(const std::shared_ptr<Particle>& p) {
                // maybe will need to ... make equality operator
                // use find to produce 2 factor
                auto& list = grid(position(*p));
                auto it = find(list.begin(), list.end(), p);
                if (it == list.end()) return false;
                else {
                    list.erase(it);
                    return true;
                }
            }
            
            void relocate(std::shared_ptr<Particle>& p, geometry::d2::f::Point center) {
                
            }
            
            void shift(std::shared_ptr<Particle>& p, Indent indent) {
                
            }
            
            // input can be particle that aren't inside grid 
            // can overload with just one particle without shared ptr
            std::vector<std::shared_ptr<Particle>> intersections(const std::shared_ptr<Particle>& p) {
                std::vector<std::shared_ptr<Particle>> result;
                auto pp = position(*p);
                auto 
                r_first = std::max(0, pp.row-1),
                c_first = std::max(0, pp.col-1),
                r_c = static_cast<Int>(grid.row_count())-1, r_last = std::min(r_c, pp.row+1),
                c_c = static_cast<Int>(grid.col_count())-1, c_last = std::min(c_c, pp.col+1);
                for (auto row = r_first; row <= r_last; ++row) {
                    for (auto col = c_first; col <= c_last; ++col) {
                        for (auto& gp : grid(row, col)) {
                            if (gp == p || !p->intersects(*gp)) continue;
                            result.push_back(gp); 
                        }
                    }
                }
                return result;
            }
            
            bool hasIntersection(const std::shared_ptr<Particle>& p) {
                bool result = false;
                auto pp = position(*p);
                auto 
                r_first = std::max(0, pp.row-1),
                c_first = std::max(0, pp.col-1),
                r_c = static_cast<Int>(grid.row_count())-1, r_last = std::min(r_c, pp.row+1),
                c_c = static_cast<Int>(grid.col_count())-1, c_last = std::min(c_c, pp.col+1);
                for (auto row = r_first; row <= r_last; ++row) {
                    for (auto col = c_first; col <= c_last; ++col) {
                        for (auto& gp : grid(row, col)) {
                            if (gp == p || !p->intersects(*gp)) continue;
                            result = true;
                            goto finish; 
                        }
                    }
                }
            finish:
                return result;
            }
            
            bool hasInside(const Point p) {
                return p.x >= x_min && p.x <= x_max && p.y >= y_min && p.y <= y_max;
            }
            
        private:
            Position position(const Particle& p) {
                Point pt = p.center();
                Position pp{
                    static_cast<Int>((pt.y - y_min)/side_height), 
                    static_cast<Int>((pt.x - x_min)/side_width)
                };
                assert(!(pp.row < 0 || pp.col < 0 || pp.row >= grid.row_count() || pp.col >= grid.col_count()));
                return pp;
            } 
            
            Grid<std::list<std::shared_ptr<Particle>>> grid;
            double
            side_width, 
            side_height,
            x_min,
            x_max,
            y_min,
            y_max;
        };
    }
}


namespace ant {
    
    namespace grid {
        
        bool operator==(const Size& s_0, const Size& s_1) {
            return s_0.row == s_1.row && s_0.col == s_1.col;
        }
        
        Position operator-(const Position& p, const Indent& n) {
            return {p.row - n.row, p.col - n.col};
        }
        
        Position operator+(const Position& p, const Indent& n) {
            return {p.row + n.row, p.col + n.col};
        }        
        
        bool operator==(const Position& p_0, const Position& p_1) {
            return p_0.row == p_1.row && p_0.col == p_1.col;
        }        
        
        bool operator!=(const Position& p_0, const Position& p_1) {
            return !(p_0 == p_1);
        }
        
        bool operator==(const Region& r_0, const Region& r_1) {
            return r_0.position == r_1.position && r_0.size == r_1.size;
        }
        
    }
    
}

using namespace std;


struct Naive_v1 {
    
    void Separate(vector<::Circle>& circles) {
        
        auto max_radius = max_element(circles.begin(), circles.end(), [](::Circle& c_0, ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        
        auto comp_density = [](::Circle& c_0, ::Circle& c_1) {
            return c_0.density() < c_1.density();
        };
        
        double min_density = min_element(circles.begin(), circles.end(), comp_density)->density();
        double max_density = max_element(circles.begin(), circles.end(), comp_density)->density();        
        
        default_random_engine rng;
        
        vector<shared_ptr<::Circle>> cs(circles.size());
        vector<shared_ptr<Field::Circle>> cs_field(circles.size()); 
        Field field(Point(-3, -3), Point(4, 4), f::Size(2*max_radius, 2*max_radius));
        for (int i = 0; i < circles.size(); ++i) {
            cs[i].reset(new ::Circle(circles[i]));
            cs_field[i].reset(new Field::Circle(cs[i]));
            field.add(cs_field[i]);
        }
        
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            for (auto& c : cs_field) {
                auto ins = field.intersections(c);
                if (ins.empty()) continue;
                has_intersections = true;
                Indent shift;
                double min_overlap = 1;
                double max_overlap = 0;
                for (auto& i : ins) {
                    auto n = c->center() - i->center();
                    min_overlap = min<double>(min_overlap, n.distance());
                    max_overlap = max<double>(max_overlap, n.distance());
                    shift += n;
                } 
                shift = shift.normed();
                shift *= (max_overlap - min_overlap) * (max_density - c->circle()->density()) / (max_density - min_density) + min_overlap;
                
                field.remove(c);
                c->circle()->center += shift;
                field.add(c);
            }   
            shuffle(cs_field.begin(), cs_field.end(), rng);
        }
        
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = cs[i]->center;
        }
    }
};

static vector<double> solution(const vector<::Circle>& cs) {
    vector<double> s(2*cs.size());
    for (auto i = 0; i < cs.size(); ++i) {
        auto& p = cs[i].center;
        assert(p.x > -100 && p.x < 100 && p.y > -100 && p.y < 100);
        s[2*i]      = p.x;
        s[2*i+1]    = p.y;
    }
    return s;
}


struct CirclesSeparation {
    vector<double> minimumWork(vector<double>& x, vector<double>& y, vector<double>& r, vector<double>& m) {
        int N = (int)m.size();
        vector<::Circle> circles(N);
        vector<double> result(2*N);
        for (int i = 0; i < N; i++) {
            circles[i] = ::Circle(x[i], y[i], r[i]+1.e-9/2, m[i]);
        }
        Naive_v1 naive;
        naive.Separate(circles);
        return solution(circles);
    }
};


