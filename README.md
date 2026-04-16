# Testes Unitários

Caminho do arquivo: cd /mnt/c/Users/reiha/Workspace/t3

gcc -I include -I unity src/calc.c test/test_calc.c unity/unity.c -o run_tests

gcc -I include -I unity src/hashfile.c test/hashfile.c unity/unity.c -o run_tests
./run_tests

# Comandos

 ./ted -e /mnt/c/Users/reiha/Workspace/t3/t1 -f c1.geo -o /mnt/c/Users/reiha/Workspace/t3/output -q c1/mudanca-todos-moradores-com-verif.qry -pm c1.pm