filename = 'quizzes_solutions.txt'
solv_file = 'puzzle_solutions.txt'

puzzle_cnt = 0
sol_list = []

with open(filename, 'r') as f:
    for line in f:
        a = f.readline()
        try:
            sol_list.append(a.split(',',1)[1])
        except IndexError:
            print('puzzle_count:', puzzle_cnt)
        puzzle_cnt += 1
        if puzzle_cnt == 100:
            break

print('puzzle_count:', puzzle_cnt)

with open(solv_file, 'w') as f:
    for line in sol_list:
        f.write(line)
