# Roteiro de implementação
- Reutilizar a estrutura de Lista implementado no projeto ted2, porém utilizar apenas o necessário e retirar tudo que não precisa e for específico do jogo.
- Adaptar esta lista para receber palavras - strings.
- Criar um vetor estático da Lista.
- Implementar um algoritmo (ingênio, hash, hash duplo do slide) e testar.
- Aplicar o algoritmo na lista e fazer tratamento de colisões - achou o índice, mas este índice já tem um elemento - ocorreu uma colisão - logo inserir na lista já existente.


# Testes
- Criar um src/hashfile.c com importação do hashfile.h e as funções sem nenhuma implementação, para ao menos compilar. E então testar o comando abaixo: 
gcc -I include -I unity src/hashfile.c test/hashfile.c unity/unity.c -o  run_tests
./run_tests