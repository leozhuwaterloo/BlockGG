import sys
import os
from collections import defaultdict

def main(file_name):
    print("Formatting %s" % file_name)
    file_dir = os.path.dirname(os.path.abspath(file_name))
    file_name_no_ext = file_name.split('/')[-1].split('.')[0]
    new_file_dir = os.path.join(file_dir, file_name_no_ext)

    with open(file_name, 'r') as f:
        lines = f.readlines()
    obj_lines = defaultdict(lambda: defaultdict(list))
    cur_obj = None
    pre_lines = []
    reached_first = False
    pre_lengths = defaultdict(int)
    for line in lines:
        if line[0] == 'o':
            reached_first = True
            name = ''.join(line.split('_')[:-1]).split(' ')[1]
            if cur_obj is not None:
                for attrib in obj_lines[cur_obj]:
                    pre_lengths[attrib] += len(obj_lines[cur_obj][attrib])
            cur_obj = name
        elif line[0] in ['v', 'f']:
            attrib = line.split(' ')[0]
            if attrib == 'f':
                if '//' in line:
                    tokens = line.split(' ')
                    for i in range(1, 4):
                        new_tokens = tokens[i].split('//')
                        new_tokens[0] = int(new_tokens[0]) - pre_lengths['v']
                        new_tokens[1] = int(new_tokens[1]) - pre_lengths['vn']
                        tokens[i] = '//'.join([str(t) for t in new_tokens])
                    line = ' '.join(tokens) + '\n'
                obj_lines[cur_obj][attrib].append(line)
            elif attrib == 'v':
                tokens = line.split(' ')
                for i in range(1, 4):
                    tokens[i] = float(tokens[i]) * 0.01;
                line = ' '.join([str(t) for t in tokens]) + '\n'
                obj_lines[cur_obj][attrib].append(line)
            else:
                obj_lines[cur_obj][attrib].append(line)
        elif not reached_first and line[0] not in ['m']:
            if "OBJ File: " in line:
                line = "%s''\n" % line.split("'")[0]
            pre_lines.append(line)

    if not os.path.exists(new_file_dir):
        os.makedirs(new_file_dir)
    for name in obj_lines:
        new_file_name = '%s.obj' % name
        print("Creating %s" % new_file_name)
        with open(os.path.join(new_file_dir, new_file_name), 'w') as f:
            f.writelines(pre_lines)
            f.write("o %s\n" % name)
            f.writelines(obj_lines[name]['v'])
            f.writelines(obj_lines[name]['vt'])    
            f.writelines(obj_lines[name]['vn'])
            f.writelines(obj_lines[name]['f'])

    ans = None
    while ans is None or ans.lower() not in ['y', 'n']:
        print("Do you want to create a example lua file?")
        ans = raw_input("(Note this might override your existing lua file)(y/n)\n")
    if ans.lower() == 'y':
        new_file_name = '%s.lua' % file_name_no_ext
        print("Creating %s" % new_file_name)
        with open(os.path.join(file_dir, '../models/%s' % new_file_name), 'w ') as f:
            for i in range(len(obj_lines)):
                f.write('mat%d = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "")\n' % i)
            f.write("\nroot = gr.node('root')\n\n")
            for i, name in enumerate(obj_lines):
                f.write("%s = gr.mesh('%s', '%s')\n" % (name, name, name))
                f.write("%s:set_material(mat%d)\n" % (name, i))
                f.write("root:add_child(%s)\n\n" % name)
            f.write('return root\n')

    ans = None
    while ans is None or ans.lower() not in ['y', 'n']:
        ans = raw_input("Do you want to delete the original object (y/n)\n")
    if ans.lower() == 'y':
        os.remove(file_name)


if __name__ == '__main__':
    if len(sys.argv) < 1:
        print("Usage <file_name>")
    else:
        main(sys.argv[1])
