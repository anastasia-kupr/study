import numpy as np

matrix = np.array([
    [6, 1, 2, 3],
    [3, 4, 5, 2],
    [8, 2, 0, 9],
    [3, 4, 4, 5]
])
volume = np.array([120, 80, 160, 90])
consumption = np.array([50, 150, 120, 130])

# коэффициенты целевой функции
target = np.nan_to_num(matrix.flatten())

# # тестовые данные
# matrix = np.array([
#     [1, 2, 4, 1],
#     [2, 3, 1, 5],
#     [3, 2, 4, 4]
# ])
# volume = np.array([50, 45, 40])
# consumption = np.array([30, 30, 20, 40])
# target = np.nan_to_num(matrix.flatten())

class TransportProblemSolver:
    def __init__(self, cost=None, targ=None, volume=None, cons=None):
        self.cost = cost
        self.post = np.zeros(self.cost.shape)
        self.post.fill(np.nan)
        self.chars = np.zeros(self.cost.shape)
        self.chars.fill(np.nan)
        self.target = targ
        self.volume = volume
        self.cons = cons
        self.rows = len(self.volume)
        self.cols = len(self.cons)
        self.vp = np.zeros(self.rows)
        self.cp = np.zeros(self.cols)

    def check_type(self):
        v_sum = self.volume.sum()
        c_sum = self.cons.sum()
        if v_sum == c_sum:
            return True
        elif v_sum > c_sum:
            self.cons = np.append(self.cons, v_sum - c_sum)
            self.cost = np.append(self.cost, np.zeros((self.cost.shape[0], 1)), axis=1)
            self.post = np.append(self.post, np.zeros((self.post.shape[0], 1)), axis=1)
            self.chars = np.append(self.chars, np.zeros((self.chars.shape[0], 1)), axis=1)
        else:
            self.volume = np.append(self.volume, c_sum - v_sum)
            self.cost = np.append(self.cost, np.zeros((1, self.cost.shape[1])), axis=0)
            self.post = np.append(self.post, np.zeros((1, self.post.shape[1])), axis=0)
            self.chars = np.append(self.chars, np.zeros((1, self.chars.shape[1])), axis=0)
        self.post.fill(np.nan)
        self.chars.fill(np.nan)
        self.rows = len(self.volume)
        self.cols = len(self.cons)
        self.vp = np.zeros(self.rows)
        self.cp = np.zeros(self.cols)
        return False

    def initial_solution(self):
        pass

    def check_plan(self):
        a = self.rows * self.cols - len(np.where(np.isnan(self.post))[0])
        if a == self.cols + self.rows - 1:
            print("Plan is non-degenerate")
            return True
        else:
            print("Plan is degenerate!")
            return False

    def total_cost(self):
        z = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if not np.isnan(self.post)[i][j]:
                    z += self.post[i][j] * self.cost[i][j]
        print("Z =", z)
        return z

    def NWC(self):
        for i in range(self.rows):
            if self.volume[i] == 0:
                continue
            for j in range(self.cols):
                if self.cons[j] == 0:
                    continue
                if self.cons[j] <= self.volume[i]:
                    self.post[i][j] = self.cons[j]
                    self.cons[j] = 0
                    self.volume[i] -= self.post[i][j]
                else:
                    self.post[i][j] = self.volume[i]
                    self.volume[i] = 0
                    self.cons[j] -= self.post[i][j]
                if self.volume[i] == 0:
                    break

    def ME(self):
        while self.volume.sum() != 0 and self.cons.sum() != 0:
        # for it in range(self.rows + self.cols - 1):
            min_ind = None
            for i in range(self.rows):
                if self.volume[i] == 0:
                    continue
                for j in range(self.cols):
                    if self.cons[j] == 0:
                        continue
                    if min_ind is None:
                        min_ind = (i, j)
                    if self.cost[min_ind] >= self.cost[i][j]:
                        min_ind = (i, j)

            min_val = np.minimum(self.volume[min_ind[0]], self.cons[min_ind[1]])
            self.post[min_ind] = min_val
            self.volume[min_ind[0]] -= min_val
            self.cons[min_ind[1]] -= min_val

    def potential_method(self):
        eq = np.zeros((self.rows + self.cols, self.rows + self.cols))
        vec = np.zeros(self.rows + self.cols)
        counter = 0
        for i in range(self.rows):
            for j in range(self.cols):
                if not np.isnan(self.post[i][j]) and self.post[i][j] != 0:
                    eq[counter][i] = 1
                    eq[counter][self.rows + j] = 1
                    vec[counter] = self.cost[i][j]
                    counter += 1
        ind = 0
        while counter < (self.rows + self.cols):
            eq[counter][ind] = 1
            counter += 1
        x = np.linalg.solve(eq, vec)
        for i in range(self.rows):
            self.vp[i] = x[i]
        for i in range(self.cols):
            self.cp[i] = x[i + self.rows]

    def fill_chars(self):
        self.chars.fill(np.nan)
        for i in range(self.rows):
            for j in range(self.cols):
                if np.isnan(self.post[i][j]):
                    self.chars[i][j] = self.vp[i] + self.cp[j] - self.cost[i][j]

    def check_optimal_criteria(self):
        for i in range(self.rows):
            for j in range(self.cols):
                if not np.isnan(self.chars[i][j]):
                    if self.chars[i][j] > 0:
                        print("Non-optimal")
                        return False
        print("Optimal")
        return True

    def recycle(self):
        free_cell_ind = np.unravel_index(np.nanargmax(self.chars), self.chars.shape)
        row_cand = []
        for j in range(self.rows):
            if j == free_cell_ind[0]:
                continue
            if not np.isnan(self.post[j][free_cell_ind[1]]):
                row_cand.append(j)
        nfree_cell_ind = None
        for j in row_cand:
            for i in range(self.cols):
                if not np.isnan(self.post[j][i]):
                    if not np.isnan(self.post[free_cell_ind[0]][i]):
                        nfree_cell_ind = (j, i)
                        break
            if nfree_cell_ind is not None:
                break
        cell_ind_list =[free_cell_ind, (free_cell_ind[0], nfree_cell_ind[1]),
                         nfree_cell_ind, (nfree_cell_ind[0], free_cell_ind[1])]
        m_arr = np.array([self.post[cell_ind_list[1]], self.post[cell_ind_list[2]], self.post[cell_ind_list[3]]])
        vm = np.nanmin(m_arr)
        plus_minus = [1, -1, 1, -1]
        for (i, j), p in zip(cell_ind_list, plus_minus):
            if np.isnan(self.post[i][j]):
                self.post[i][j] = 0
            self.post[i][j] += p * vm

    def fit(self):
        self.check_type()
        # self.NWC()
        self.ME()
        print(self)
        if self.check_plan():
            cond = False
            while not cond:
                self.total_cost()
                self.potential_method()
                self.fill_chars()
                print(self)
                cond = self.check_optimal_criteria()
                if cond:
                    break
                self.recycle()
                print(self)
        else:
            pass

    def __str__(self):
        def rstr(k):
            return str(np.round(k, decimals=2))

        s = "\x1b[31;1m" +"\t"
        for i in range(self.cols):
            s += "B" +rstr(i) + "  \t"
        s += "\t\x1b[0m" + "\n"
        for i in range(self.rows):
            s += "\x1b[31;1m" + "A" + rstr(i) + "\x1b[0m" + "\t"
            for j in self.cost[i]:
                s += rstr(j) + "  \t"
            s += "\n  \t"
            for j in self.post[i]:
                s += "\x1b[32;1m" + "  " + rstr(j) + "\x1b[0m" + "\t"
            s += "\x1b[34;1m" + rstr(self.volume[i]) + "\x1b[0m" + "\t"
            s += "\x1b[1;36;1m" + rstr(self.vp[i]) + "\x1b[0m" + "\n"
            #s += "  \t" + "\x1b[0;35m"
            #for j in self.chars[i]:
            #    s += "  " + rstr(j) + "  \t"
            #s += "\x1b[0m" + "\n"
        s += "  \t" + "\x1b[34;1m"
        for i in self.cons:
            s += "  " + rstr(i) + "  \t"
        s += "\x1b[0m" + "\n  \t" + "\x1b[1;36;1m"
        for i in self.cp:
            s += "  " + rstr(i) + "  \t"
        s += "\x1b[0m" + "\n\n" + "\x1b[0;35m"
        for i in range(self.rows):
            s += "\t"
            for j in self.chars[i]:
                s += "  " +rstr(j) + "  \t"
            s += "\n"
        s += "\x1b[0m"
        return s


slvr = TransportProblemSolver(cost=matrix, targ=target, volume=volume, cons=consumption)
slvr.fit()

