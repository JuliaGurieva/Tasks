# LRU Cache

В h-файле реализован "LRU cache"- ассоциативный контейнер, вытесняющий элементы по алгоритму Least Recently Used. Тесты содержатся с cpp-файле и были выданы вместе с задачей.

Пока размер кеша не превышает максимальный, он работает как обычный map:

 * Метод "Set()" запоминает новое значение соответствующее ключу.
 * Метод "Get()" возвращает false, если по ключу ничего не положили. Если ключу соответствует какое-то значение, то Get() возвращает true и это значение.
Если после Set() размер кеша превышает max_size, то срабатывает алгоритм LRU и кеш удаляет из себя ключ, к которому обращались раньше всего (в Get() или Set()).
Вычислительная сложность методов Get() и Set() в среднем O(1).


 Для решения используются контейнеры -- list и unordered_map, в list хранятся значения, в map - ключи и итераторы листа.