#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

template<class T>
class Matrix {
private:
    size_t n = 0, m = 0;
    std::vector<std::vector<T>> data;

public:
    using Row = std::vector<T>;
    using iterator = typename std::vector<std::vector<T>>::iterator;
    using const_iterator = typename std::vector<std::vector<T>>::const_iterator;
    Matrix(const std::vector<std::vector<T>>& a = {{}}) {
        n = a.size();
        if (n > 0) {
            m = a[0].size();
        }
        data = std::vector<std::vector<T>>(n, std::vector<T>(m));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                data[i][j] = a[i][j];
            }
        }
    }
    std::pair<size_t, size_t> size() const {
        return {n, m};
    }

    T GetByIndex(size_t x, size_t y) const {
        return data[x][y];
    }
    Matrix<T>& operator+= (const Matrix<T>& other) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                data[i][j] += other.data[i][j];
            }
        }
        return *this;
    }
    Matrix<T> operator+ (const Matrix<T>& other) const {
        Matrix<T> tmp(data);
        tmp += other;
        return tmp;
    }
    template<class V>
    Matrix<T>& operator*= (const V& scalar) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }
    template<class V>
    Matrix<T> operator* (const V& scalar) const {
        Matrix<T> tmp(data);
        tmp *= scalar;
        return tmp;
    }
    Matrix<T>& transpose() {
        Matrix<T> tmp(data);
        data = std::vector<std::vector<T>>(m, std::vector<T>(n));
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                data[i][j] = tmp.data[j][i];
            }
        }
        std::swap(n, m);
        return *this;
    }
    Matrix<T> transposed() const {
        Matrix<T> tmp(data);
        tmp.transpose();
        return tmp;
    }

    Matrix<T>& operator*= (const Matrix<T>& other) {
        auto [n_other, m_other] = other.size();
        assert(m == n_other);
        Matrix<T> tmp(data);
        data = std::vector<std::vector<T>>(n, std::vector<T>(m_other, 0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t z = 0; z < m_other; ++z) {
                for (size_t j = 0; j < m; ++j) {
                    data[i][z] += tmp.data[i][j] * other.data[j][z];
                }
            }
        }
        m = m_other;
        return *this;
    }

    Matrix<T> operator* (const Matrix<T>& other) const {
        auto [n_other, m_other] = other.size();
        assert(m == n_other);
        Matrix<T> tmp(std::vector<std::vector<T>>(n, std::vector<T>(m_other, 0)));
        for (size_t i = 0; i < n; ++i) {
            for (size_t z = 0; z < m_other; ++z) {
                for (size_t j = 0; j < m; ++j) {
                    tmp.data[i][z] += this->data[i][j] * other.data[j][z];
                }
            }
        }
        return tmp;
    }

    T getDeterminant() const {
        auto matrix = data;
        assert(m == n);
        T determinant = 1;
        for (int i = 0; i < n; ++i) {
            int k = i;
            for (int j = i + 1; j < n; ++j)
                if (abs(matrix[j][i]) > abs(matrix[k][i]))
                    k = j;
            swap (matrix[i], matrix[k]);
            if (matrix[i][i] == 0) {
                determinant = 0;
                break;
            }
            if (i != k) {
                determinant = -1 * determinant;
            }
            determinant *= matrix[i][i];
            for (int j = i + 1; j < n; ++j)
                matrix[i][j] /= matrix[i][i];
            for (int j = 0; j < n; ++j)
                if (j != i)
                    for (int k = i + 1; k < n; ++k)
                        matrix[j][k] -= matrix[i][k] * matrix[j][i];
        }
        return determinant;
    }
    constexpr iterator begin() noexcept {
        return data.begin();
    }
    constexpr const_iterator begin() const noexcept {
        return data.begin();
    }
    constexpr iterator end() noexcept {
        return data.end();
    }
    constexpr const_iterator end() const noexcept {
        return data.end();
    }
    const_iterator cbegin() const {
        return data.cbegin();
    }
    const_iterator cend() const {
        return data.cend();
    }
};



template<class T>
std::ostream& operator<< (std::ostream& out, const Matrix<T>& mat) {
    auto [n, m] = mat.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            out << mat.GetByIndex(i, j);
            if (j + 1 != m) {
                out << '\t';
            }
        }
        if (i + 1 != n) {
            out << '\n';
        }
    }

    return out;
}

template<class T>
std::ostream& operator<< (std::ostream& out, std::vector<T> It) {
    auto first = It.begin();
    auto last = It.end();
    while (first != last) {
        out << *first;
        ++first;
        if (first != last) {
            out << " ";
        }
    }
    return out;
}
