def split_name(name):
    res = ""
    for i, char in enumerate(name):
        if char.isupper() and i != 0:
            res += ' '
        res += char

    return res

def lower_name(name):
    return name[0].lower() + name[1:]
