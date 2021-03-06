# КЧ, АВЛ деревья и их тестирование

## АВЛ-дерево

![insert](https://github.com/sedykh-ag/trees/blob/main/Presentation/AVL_tree_slide.jpg)

## Красно-черное дерево

![insert](https://github.com/sedykh-ag/trees/blob/main/Presentation/rb_tree_slide.png)

## Профайлер

В ходе тестирования измерялась зависимость времени вставки(insert), поиска(find) и удаления(erase) от количества элементов в дереве N. Предполагаемая асимптотика - O(log N), что было подтверждено в пределах погрешности результатами измерений и их линейной апроксимацией.

Измерение зависимости времени от количества элементов осуществлялось с помощью данного цикла:

![insert](https://github.com/sedykh-ag/trees/blob/main/Presentation/Profiler_code.png)

## Результаты

### АВЛ-дерево

![insert](https://github.com/sedykh-ag/trees/blob/main/Profiler/AVL_tree_insert.png)

![find](https://github.com/sedykh-ag/trees/blob/main/Profiler/AVL_tree_find.png)

![remove](https://github.com/sedykh-ag/trees/blob/main/Profiler/AVL_tree_erase.png)

### Красно-чёрное дерево

![insert](https://github.com/sedykh-ag/trees/blob/main/Profiler/RB_tre_insert.png)

![find](https://github.com/sedykh-ag/trees/blob/main/Profiler/RB_tree_find.png)

![erase](https://github.com/sedykh-ag/trees/blob/main/Profiler/RB_tree_erase.png)
