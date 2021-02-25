---------------------------------------------------------------
-- file: cu.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: Control Unit (CU)
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cu is
    port (
        state      :  in unsigned(1 downto 0) := "00";
        instr      :  in unsigned(15 downto 0) := "0000000000000000";
        nop_op     : out std_logic := '0';
        jr_op      : out std_logic := '0';
        jmp_op     : out std_logic := '0';
        add_op     : out std_logic := '0';
        sub_op     : out std_logic := '0';
        mul_op     : out std_logic := '0';
        cmp_op     : out std_logic := '0';
        be_op      : out std_logic := '0';
        blt_op     : out std_logic := '0';
        mov_op     : out std_logic := '0';
        ld_op      : out std_logic := '0';
        st_op      : out std_logic := '0';
        const_load : out std_logic := '0';
        pc_wr_en   : out std_logic := '0';
        excpt      : out std_logic := '0';
        registers_wr_en : out std_logic := '0';
        flags_wr_en     : out std_logic := '0'
    );
end entity;

architecture a_cu of cu is
    signal opcode : unsigned(3 downto 0) := "0000";
    signal nop_op_s     : std_logic := '0';
    signal jr_op_s      : std_logic := '0';
    signal jmp_op_s     : std_logic := '0';
    signal add_op_s     : std_logic := '0';
    signal sub_op_s     : std_logic := '0';
    signal mul_op_s     : std_logic := '0';
    signal cmp_op_s     : std_logic := '0';
    signal be_op_s      : std_logic := '0';
    signal blt_op_s     : std_logic := '0';
    signal mov_op_s     : std_logic := '0';
    signal ld_op_s      : std_logic := '0';
    signal st_op_s      : std_logic := '0';
    signal const_load_s : std_logic := '0';
    signal pc_wr_en_s   : std_logic := '0';
    signal excpt_s      : std_logic := '0';
    signal registers_wr_en_s : std_logic := '0';
    signal flags_wr_en_s : std_logic := '0';
begin
    -- extract instruction 
    opcode <= instr(15 downto 12);

    nop_op_s  <= '1' when opcode = "0000" else
                 '0';
    add_op_s  <= '1' when opcode = "0001" else
                 '0';
    sub_op_s  <= '1' when opcode = "0010" else
                 '0';
    mul_op_s  <= '1' when opcode = "0011" else
                 '0';
    cmp_op_s  <= '1' when opcode = "0100" else
                 '0';
    be_op_s   <= '1' when opcode = "0101" else
                 '0';
    blt_op_s  <= '1' when opcode = "0110" else
                 '0';
    jr_op_s <=   '1' when opcode = "0111" else
                 '0';
    jmp_op_s <=  '1' when opcode = "1100" else
                 '0';
    mov_op_s  <= '1' when opcode = "1000" else
                 '0';
    const_load_s <= '1' when opcode = "1001" else
                  '0';
    ld_op_s  <= '1' when opcode = "1010" else
                 '0';
    st_op_s  <= '1' when opcode = "1011" else
                 '0';
    pc_wr_en_s <= '1' when state = "10" and excpt_s = '0' else
                  '0';
    excpt_s <= '0' when opcode = "0000"
                     or opcode = "0001"
                     or opcode = "0010"
                     or opcode = "0011"
                     or opcode = "0100"
                     or opcode = "0101"
                     or opcode = "0110"
                     or opcode = "0111"
                     or opcode = "1000"
                     or opcode = "1001"
                     or opcode = "1010"
                     or opcode = "1011"
                     or opcode = "1100"
                    else '1';

    -- indica se o banco será escrito
    registers_wr_en_s <= '1' when state = "10" and (
                             mov_op_s     = '1' or 
                             const_load_s = '1' or
                             add_op_s     = '1' or 
                             sub_op_s     = '1' or 
                             mul_op_s     = '1' or
                             ld_op_s      = '1') else
                         '0';

    flags_wr_en_s <= '1' when (add_op_s = '1' or sub_op_s = '1' or cmp_op_s = '1') and state = "10" 
                         else '0';

    nop_op     <= nop_op_s;
    jr_op      <= jr_op_s;
    jmp_op     <= jmp_op_s;
    pc_wr_en   <= pc_wr_en_s;
    add_op     <= add_op_s;
    sub_op     <= sub_op_s;
    mul_op     <= mul_op_s;
    cmp_op     <= cmp_op_s;
    be_op      <= be_op_s;
    blt_op     <= blt_op_s;
    mov_op     <= mov_op_s;
    ld_op      <= ld_op_s;
    st_op      <= st_op_s;
    const_load <= const_load_s;
    excpt      <= excpt_s;
    registers_wr_en <= registers_wr_en_s;
    flags_wr_en <= flags_wr_en_s;
end architecture;
