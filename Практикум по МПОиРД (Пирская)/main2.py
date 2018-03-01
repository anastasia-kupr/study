import numpy as np

# Problem No. 6

# Initial conditions
c = np.array([9., 6., 4., 7., 0., 0., 0.])
A0 = np.array([180., 210., 800.])
a = np.array([
    [1., 0., 2., 1., 1., 0., 0.],
    [0., 1., 3., 2., 0., 1., 0.],
    [4., 2., 0., 4., 0., 0., 1.]
])

# # test primera iz methodы
# c = np.array([3., 4., 0., 0., 0., 0.])
# A0 = np.array([9., 13., 1., 2.])
# a = np.array([
#     [2., 3., 1., 0., 0., 0.],
#     [3., 2., 0., 1., 0., 0.],
#     [1., -1., 0., 0., 1., 0.],
#     [0., 1., 0., 0., 0., 1.]
# ])

# флаг печати таблиц после каждой итерации
PRNT_DBG = False

class SimplexTable:
    # инициализация по умолчанию
    def __init__(self):
        self.free_cnt = 0
        self.basis_cnt = 0
        self.free_coef = np.array([])
        self.basis_coef = np.array([])
        self.cond_table = np.array([])
        self.cond_coef = np.array([])
        self.delta_coef = np.array([])
        self.Z = 0
        self.simplex_div = np.array([])
        self.basis_indexes = np.array([])

    # инициализация
    def init(self, free_coef, cond_table, cond_coef):
        self.free_cnt = cond_table.shape[1] - cond_table.shape[0]
        self.basis_cnt = cond_table.shape[0]
        self.free_coef = free_coef
        self.basis_coef = np.array(free_coef[-self.basis_cnt:])
        self.cond_table = cond_table
        self.cond_coef = cond_coef
        self.delta_coef = np.zeros(self.free_cnt + self.basis_cnt)
        self.simplex_div = np.zeros(self.basis_cnt)
        self.basis_indexes = np.zeros(self.basis_cnt)
        for i in range(self.basis_cnt):
            self.simplex_div[i] = np.nan
            self.basis_indexes[i] = self.free_cnt + i

        self.iter_cnt = 0

    # вычисление значения максимизируемой функции
    def calc_z(self):
        self.Z = 0
        for b, A in zip(self.basis_coef, self.cond_coef):
            self.Z += b * A

    # вычисление элементов строки оценок
    def calc_delta(self):
        for i, c in enumerate(self.free_coef):
            self.delta_coef[i] = self.Z - c

    # нахождение разрешающего элемента и симплексных отношений
    def find_resolving_element(self):
        # индекс разрешающего столбца
        r_col = np.argmin(self.delta_coef)
        for i, ac in enumerate(self.cond_coef):
            if self.cond_table[i][r_col] == 0:
                self.simplex_div[i] = np.nan
                continue
            self.simplex_div[i] = ac / self.cond_table[i][r_col]
            if self.simplex_div[i] < 0:
                self.simplex_div[i] = np.nan
        r_row = np.nanargmin(self.simplex_div)

        return self.cond_table[r_row][r_col], r_row, r_col

    # правило треуголника пересчета таблицы
    def triangle_rule(self, res_elem, res_row, res_col):
        new_table = np.zeros(self.cond_table.shape)
        for i in range(self.cond_table.shape[0]):
            if i == res_row:
                new_table[i] = self.cond_table[i]
                continue
            for j in range(self.cond_table.shape[1]):
                if j == res_col:
                    new_table[i][j] = 0
                    continue
                new_table[i][j] = self.cond_table[i][j] - (self.cond_table[res_row][j] *\
                    self.cond_table[i][res_col]) / res_elem
        self.cond_table = new_table

    # пересчет симплекс таблицы
    def recalculate_table(self, res_elem, res_row, res_col):
        # заменяем базисную переменную
        self.basis_indexes[res_row] = res_col
        self.basis_coef[res_row] = self.free_coef[res_col]

        res_cond = self.cond_coef[res_row]

        # по правилу треугольника пересчитываем коэффициенты ограничивающих условий
        for i in range(self.basis_cnt):
            if i == res_row:
                continue
            self.cond_coef[i] -= res_cond * self.cond_table[i][res_col] / res_elem

        self.triangle_rule(res_elem, res_row, res_col)
        self.cond_coef[res_row] /= res_elem
        self.cond_table[res_row] /= res_elem

    # перерасчет строки оценок
    def recalculate_delta(self, res_elem, res_row, res_col):
        res_delta = self.delta_coef[res_col]
        for i in range(self.free_cnt + self.basis_cnt):
            if i == res_col:
                self.delta_coef[i] = 0
                continue
            self.delta_coef[i] -= self.cond_table[res_row][i] * res_delta / res_elem

    # проверка условия выхода - положительности строки оценок
    def check_delta(self):
        return np.all(self.delta_coef >= 0)

    # функция одной итерации
    def iterate(self):
        if PRNT_DBG:
            print("\x1b[31;1m" + str(self.iter_cnt) + "\x1b[0m")
        res = False
        if self.check_delta():
            res = True
        e, r, c = self.find_resolving_element()
        if PRNT_DBG:
            print(self)
        self.recalculate_delta(e, r, c)
        self.recalculate_table(e, r, c)
        self.calc_z()
        if res:
            self.print_res()
        self.iter_cnt += 1
        return res

    # главная функция выполняющая максимизацию по методу симплекс таблицы
    def fit(self):
        self.calc_z()
        self.calc_delta()
        e, r, c = self.find_resolving_element()
        print("\x1b[31;1m" + str(self.iter_cnt) + "\x1b[0m")
        print(self)
        self.recalculate_table(e, r, c)
        self.iter_cnt = 1
        self.calc_z()
        while not self.iterate():
            pass

    # функция печати результата
    def print_res(self):
        print(self)
        res_str = "SOLVED: f = " + str(self.Z) + " at ("
        for i in range(self.free_cnt):
            if i >= self.basis_cnt:
                res_str += '0'
                continue
            if len(self.cond_coef[np.argwhere(self.basis_indexes == i)]) != 0:
                res_str += str(self.cond_coef[np.argwhere(self.basis_indexes == i)][0][0])
            else:
                res_str += '0'
            res_str += ', '
        res_str += ")\n"
        print(res_str)

    # перегрузка функции печати для класса
    def __str__(self):
        s = "\x1b[31;1m" + "B_i\tC_b\tA_j\t" + "\x1b[0m"
        for i in self.free_coef:
            s += "\x1b[32;1m" + str(i) + "\x1b[0m" + "\t"
        s += "\x1b[31;1m" + "Simplex\n" + "\x1b[0m"
        for i in range(self.cond_table.shape[0]):
            s += "\x1b[32;1m" + str(self.basis_indexes[i]) + "\t" + str(self.basis_coef[i]) + "\x1b[0m" + "\t" +\
                str(self.cond_coef[i]) + "\t"
            for j in range(self.cond_table.shape[1]):
                s += str(self.cond_table[i][j]) + "\t"
            s += "\x1b[34;1m" + str(self.simplex_div[i]) + "\x1b[0m" + "\n"
        s += "\x1b[31;1m" + "   \tZ_j\t" + "\x1b[0m" + "\x1b[34;1m" + str(self.Z)
        for i in self.delta_coef:
            s += "\t" + str(i)
        s += "\x1b[0m" + "\n"
        return s

st = SimplexTable()
st.init(c, a, A0)
st.fit()
