def split_name(name):
    res = ""
    length = len(name)
    
    for i, char in enumerate(name):
        if i > 0:
            prev_char = name[i - 1]
            next_char = name[i + 1] if i + 1 < length else ''
            
            if char.isupper() and (prev_char.islower() or (prev_char.isupper() and next_char.islower())):
                res += ' '
        
        res += char
    
    return res

def lower_name(name):
    return name[0].lower() + name[1:]
