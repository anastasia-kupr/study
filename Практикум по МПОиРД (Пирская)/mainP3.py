import numpy as np
from copy import deepcopy as dc

matrix = np.array([
    [6, 1, 2, 3],
    [3, 4, 5, 2],
    [8, 2, 27, 9],
    [3, 4, 4, 5]
])
volume = np.array([120, 80, 160, 90])
consumption = np.array([50, 150, 120, 130])

# # тестовые данные
# matrix = np.array([
#     [1, 2, 4, 1],
#     [2, 3, 1, 5],
#     [3, 2, 4, 4]
# ])
# volume = np.array([50, 45, 40])
# consumption = np.array([30, 30, 20, 40])

class TransportProblemSolver:
    def __init__(self, cost=None, volume=None, cons=None):
        self.cost = cost
        self.post = np.zeros(self.cost.shape)
        self.post.fill(np.nan)
        self.chars = np.zeros(self.cost.shape)
        self.chars.fill(np.nan)
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
        p = self.NWC(solve=False)
        temp_post = dc(self.post)
        temp_vol = dc(self.volume)
        temp_cons = dc(self.cons)
        self.post = dc(p)
        self.volume.fill(0)
        self.cons.fill(0)
        print(self)
        self.post = dc(temp_post)
        self.volume = dc(temp_vol)
        self.cons = dc(temp_cons)
        nwc_score = self.total_cost(post=p, prnt="North-West Corner")
        p1 = self.ME(solve=False)
        temp_post = dc(self.post)
        temp_vol = dc(self.volume)
        temp_cons = dc(self.cons)
        self.post = dc(p1)
        self.volume.fill(0)
        self.cons.fill(0)
        print(self)
        self.post = dc(temp_post)
        self.volume = dc(temp_vol)
        self.cons = dc(temp_cons)
        me_score = self.total_cost(post=p1, prnt="Minimal Element")
        if me_score <= nwc_score:
            self.ME()
        else:
            self.NWC()

    def check_plan(self):
        a = self.rows * self.cols - len(np.where(np.isnan(self.post))[0])
        if a == self.cols + self.rows - 1:
            print("Plan is non-degenerate")
            return True
        else:
            print("Plan is degenerate!")
            return False

    def total_cost(self, post=None, prnt=None):
        z = 0
        if post is None:
            for i in range(self.rows):
                for j in range(self.cols):
                    if not np.isnan(self.post)[i][j]:
                        z += self.post[i][j] * self.cost[i][j]
        else:
            for i in range(self.rows):
                for j in range(self.cols):
                    if not np.isnan(post)[i][j]:
                        z += post[i][j] * self.cost[i][j]
        if prnt is None:
            print("Z =", z)
        else:
            print(prnt, "Z =", z)
        return z

    def NWC(self, solve=True):
        post = dc(self.post)
        cons = dc(self.cons)
        volume = dc(self.volume)

        for i in range(self.rows):
            if volume[i] == 0:
                continue
            for j in range(self.cols):
                if np.isnan(self.cost[i][j]):
                    continue
                if cons[j] == 0:
                    continue
                if cons[j] <= volume[i]:
                    post[i][j] = cons[j]
                    cons[j] = 0
                    volume[i] -= post[i][j]
                else:
                    post[i][j] = volume[i]
                    volume[i] = 0
                    cons[j] -= post[i][j]
                if volume[i] == 0:
                    break
        if not solve:
            return post
        else:
            self.post = dc(post)
            self.cons = dc(cons)
            self.volume = dc(volume)

    def ME(self, solve=True):
        post = dc(self.post)
        cons = dc(self.cons)
        volume = dc(self.volume)

        while volume.sum() != 0 and cons.sum() != 0:
            min_ind = None
            for i in range(self.rows):
                if volume[i] == 0:
                    continue
                for j in range(self.cols):
                    if np.isnan(self.cost[i][j]):
                        continue
                    if cons[j] == 0:
                        continue
                    if min_ind is None:
                        min_ind = (i, j)
                    if self.cost[min_ind] >= self.cost[i][j]:
                        min_ind = (i, j)

            min_val = np.minimum(volume[min_ind[0]], cons[min_ind[1]])
            post[min_ind] = min_val
            volume[min_ind[0]] -= min_val
            cons[min_ind[1]] -= min_val
        if not solve:
            return post
        else:
            self.post = dc(post)
            self.cons = dc(cons)
            self.volume = dc(volume)

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

    def recycle(self, dbg=False):
        free_cell_ind = np.unravel_index(np.nanargmax(self.chars), self.chars.shape)
        if dbg:
            print(free_cell_ind)
        row_cand = []
        for j in range(self.rows):
            if j == free_cell_ind[0]:
                continue
            if not np.isnan(self.post[j][free_cell_ind[1]]):
                row_cand.append(j)
        if dbg:
            print(row_cand)
        nfree_cell_ind = None
        for j in row_cand:
            for i in range(self.cols):
                if not np.isnan(self.post[j][i]):
                    if not np.isnan(self.post[free_cell_ind[0]][i]):
                        nfree_cell_ind = (j, i)
                        break
            if nfree_cell_ind is not None:
                break
        if dbg:
            print(nfree_cell_ind)
        cell_ind_list =[free_cell_ind, (free_cell_ind[0], nfree_cell_ind[1]),
                         nfree_cell_ind, (nfree_cell_ind[0], free_cell_ind[1])]
        if dbg:
            print(cell_ind_list)
        m_arr = np.array([self.post[cell_ind_list[1]], self.post[cell_ind_list[3]]])
        vm = np.nanmin(m_arr)
        if dbg:
            print(vm)
        plus_minus = [1, -1, 1, -1]
        for (i, j), p in zip(cell_ind_list, plus_minus):
            if np.isnan(self.post[i][j]):
                self.post[i][j] = 0
            self.post[i][j] += p * vm

    def fit(self):
        self.check_type()
        self.initial_solution()
        #print(self)
        if self.check_plan():
            cond = False
            while not cond:
                self.total_cost()
                self.potential_method()
                self.fill_chars()
                print(self)
                cond = self.check_optimal_criteria()
                if cond:
                    self.print_coef()
                    break
                self.recycle()
                print(self)
        else:
            pass

    def print_coef(self):
        st = "Target function coeffs:\n"
        for i in range(self.rows):
            for j in range(self.cols):
                if self.post[i][j] != 0 and not np.isnan(self.post[i][j]):
                    st += "x[" + str(i + 1) + "][" + str(j + 1) + "]\t"
        print(st)

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
                s += rstr(j) + "   \t"
            s += "\n  \t"
            for j in self.post[i]:
                s += "\x1b[32;1m" + "  " + rstr(j) + "\x1b[0m" + "\t"
            s += "\x1b[34;1m" + rstr(self.volume[i]) + "\x1b[0m" + "\t"
            s += "\x1b[1;36;1m" + rstr(self.vp[i]) + "\x1b[0m" + "\n"
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


slvr = TransportProblemSolver(cost=matrix, volume=volume, cons=consumption)
slvr.fit()

