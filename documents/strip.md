# Strip

## property

### struct stripData m_stripData

The strip's data.

### m_flagTriggered

Normal on **false**, when select on strip, set value **true**, the triggered event occur. After events happened, the flag back to **false**.

### m_flagSelected

Describe the strip's status: **selected** or **not selected**.

### m_flagClickable

The strip on eidt mode. the flag set **false**, the strip cann't trigger *mousePressEvent*.



## public function

### QPushButton *buttonAffirm() const

Return the pointer of Affirm button.

### QPushBtton *buttonCannel() const

return the pointer of Cannel button.

### int comboLeftIndex() const

Return the index of left comboBox.

### int comboRightIndex() const

return the index of right comboBox.

### QString content() const

return the texture of the LineEdit.

### bool isTriggered() const

return the flag triggered's value.

### void resetTrigger()

reset flag triggered to false.

### void setValue(int LeftIndex, int RightIndex, const QString& content)

set the index of Left comboBox, the index of Right comboBox and the content of LineEdit. 

### void toSelected()

