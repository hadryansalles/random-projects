---------------------------------------------------------------
-- file: rom.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: Read 0nly Memory (ROM)
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity rom is
    -- ROM interface
    port (
        clk : in std_logic := '0';
        address : in unsigned(6 downto 0) := "0000000";
        data_out : out unsigned(15 downto 0) := B"0000_0000_0000_0000"
    ); 
end entity;


-- RRR  = address of reg1
-- rrr  = address of reg2
-- www  = address of reg3

-- NOP             = 0000_000_000_000_000
-- ADD reg1, reg2  = 0001_000_000_rrr_RRR -- reg2 <= reg2 + reg1
-- SUB reg1, reg2  = 0010_000_000_rrr_RRR -- reg2 <= reg2 - reg1
-- MUL reg1, reg2  = 0011_000_000_rrr_RRR -- reg2 <= reg2 * reg1 
-- CMP reg1, reg2  = 0100_000_000_rrr_RRR -- reg2 - reg1 and set flags
-- BE  reg1, reg2  = 0101_AAA_AAA_AAA_AAA -- PC + AAA_AAA_AAA_AAA
-- BLT reg1, reg2  = 0110_AAA_AAA_AAA_AAA -- PC + AAA_AAA_AAA_AAA
-- JR  imm12       = 0111_AAA_AAA_AAA_AAA -- PC <= PC + AAA_AAA_AAA_AAA
-- JMP disp9[reg1] = 1100_DDD_DDD_DDD_RRR -- PC <= reg1 + disp9
-- MOV reg1, reg2  = 1000_000_000_rrr_RRR -- reg2 <= reg1
-- MOV imm9, reg1  = 1001_iii_iii_iii_RRR -- reg1 <= imm9
-- LD.W disp6[reg1], reg2 = 1010_ddd_ddd_rrr_RRR -- reg2 <= RAM(reg1+disp6)
-- ST.W reg2, disp6[reg1] = 1011_ddd_ddd_rrr_RRR -- RAM(reg1+disp6) <= reg2

-- OBSERVACAO: A instrução MUL original no V850 é:
-- MUL reg1, reg2, reg3 
-- reg3 <= (reg1*reg2)(31 downto 16)
-- reg2 <= (reg1*reg2)(15 downto 0)
-- Porém, como conversado com o professor, não há necessidade de fazer a instrução dessa forma.
-- Além da instrução não ser necessária, ela precisaria de um estado adicional na máquina de estados 
-- para escrever em dois registradores.

architecture a_rom of rom is
    type mem is array(0 to 127) of unsigned(15 downto 0);
    constant rom_data0 : mem := (
        0  => B"0000_000_000_000_000", -- NOP
        1  => B"1001_111_111_111_001", -- MOV 511, $1
        2  => B"1001_101_010_101_010", -- MOV 341, $2
        3  => B"1000_000_000_011_001", -- MOV $1, $3
        4  => B"0001_000_000_011_010", -- ADD $2, $3
        5  => B"0010_000_000_011_001", -- SUB $1, $3
        6  => B"0000_000_000_000_000", -- NOP
        7  => B"0100_000_000_010_011", -- CMP $3, $2
        8  => B"0101_000_000_000_010", -- BE  2  -- pula a prox instrução
        9  => B"0111_111_111_111_011", -- JR -5
        10 => B"0000_000_000_000_000", -- NOP
        11 => B"0100_000_000_000_011", -- CMP $3, $0
        12 => B"0101_000_000_001_111", -- BE  15 -- não faz nada
        13 => B"0011_000_000_001_011", -- MUL $3, $1
        others => (others => '0')
    );
    constant rom_lab5 : mem := (
        0  => B"1001_000_000_101_011", -- MOV  5, $3  -- 1
        1  => B"1001_000_001_000_100", -- MOV  8, $4  -- 2
        2  => B"1000_000_000_101_011", -- MOV $3, $5  -- 3
        3  => B"0001_000_000_101_100", -- ADD $4, $5  -- 3
        4  => B"1001_000_000_001_001", -- MOV  1, $1  -- 4
        5  => B"0010_000_000_101_001", -- SUB $1, $5  -- 4
        6  => B"0111_000_000_001_110", -- JR  14      -- 5
        20 => B"1000_000_000_011_101", -- MOV $5, $3  -- 6
        21 => B"0111_111_111_101_101", -- JR  -19     -- 7
        others => (others => '0')
    );
    constant rom_lab6 : mem := (
        0 => B"1001_000_000_000_011", -- MOV 0, $3            -- 1
        1 => B"1001_000_000_000_100", -- MOV 0, $4            -- 2
        2 => B"1001_000_000_001_110", -- MOV 1, $6            -- preparing to step 4
        3 => B"1001_000_011_110_111", -- MOV 30, $7           -- preparing to step 5
        4 => B"0001_000_000_100_011", -- ADD $3, $4           -- 3
        5 => B"0001_000_000_011_110", -- ADD $6, $3           -- 4
        6 => B"0100_000_000_011_111", -- CMP $7, $3           -- 5
        7 => B"0110_111_111_111_101", -- BLT -3 ; jr to 4   -- 5
        8 => B"1000_000_000_101_100", -- MOV $4, $5           -- 6
        others => (others => '0')
    );
    constant rom_ram_test : mem := (
        -- store some data
        0  => B"1001_000_100_000_001", -- MOV 32, $1
        1  => B"1001_000_001_100_010", -- MOV 12, $2
        2  => B"1011_000_000_001_010", -- ST.W  $1, 0[$2] -- RAM(12 + 0) = 32
        3  => B"1001_001_000_000_011", -- MOV 64, $3 
        4  => B"1001_000_011_000_100", -- MOV 24, $4
        5  => B"1011_000_000_011_100", -- ST.W  $3, 0[$4] -- RAM(24 + 0) = 64
        6  => B"1001_000_010_000_001", -- MOV 16, $1
        7  => B"1001_000_001_010_011", -- MOV 10, $3
        8  => B"1001_000_001_111_111", -- MOV 15, $7
        9  => B"1011_000_000_011_001", -- ST.W  $3, 0[$1] -- RAM(16) = 10
        10 => B"1011_001_000_111_001", -- ST.W  $7, 8[$1] -- RAM(16+8) = 15
        11 => B"1011_111_001_010_001", -- ST.W  $2,-7[$1] -- RAM(16-7) = 12
        -- clear all registers
        12 => B"1000_000_000_001_000", -- MOV $0, $1
        13 => B"1000_000_000_010_000", -- MOV $0, $2
        14 => B"1000_000_000_011_000", -- MOV $0, $3
        15 => B"1000_000_000_100_000", -- MOV $0, $4
        16 => B"1000_000_000_101_000", -- MOV $0, $5
        17 => B"1000_000_000_110_000", -- MOV $0, $6
        18 => B"1000_000_000_111_000", -- MOV $0, $7
        -- read from ram
        19 => B"1001_000_001_100_010", -- MOV 12, $2
        20 => B"1001_000_011_000_100", -- MOV 24, $4
        21 => B"1001_000_010_000_110", -- MOV 16, $6
        22 => B"1010_000_000_011_010", -- LD.W  0[$2], $3 -- $3 = RAM(12 + 0)
        23 => B"1010_000_000_101_100", -- LD.W  0[$4], $5 -- $5 = RAM(24 + 0)
        24 => B"1010_000_000_111_110", -- LD.W  0[$6], $7 -- $7 = RAM(16 + 0)
        25 => B"1010_001_100_011_010", -- LD.W 12[$2], $3 -- $3 = RAM(12 + 12)
        26 => B"1010_110_100_111_100", -- LD.W -12[$4], $7 -- $7 = RAM(24 - 12)
        others => (others => '0') 
    );

    constant rom_primos : mem := (
         0 => "1001000000001110", -- MOV   1, r6
         1 => "1001000110010010", -- MOV   50, r2
         2 => "1001000000001011", -- MOV    1, r3
         3 => "1001000000001001", -- MOV    1, r1
         -- loop0: 
         4 => "0001000000001011", -- ADD   r3, r1
         5 => "1011000000001001", -- ST.W  r1, 0[r1]
         6 => "0100000000001010", -- CMP   r2, r1
         7 => "0110111111111101", -- BLT   loop0
         8 => "1001000000010011", -- MOV    2, r3
         -- loop1: 
         9 => "1001000001011111", -- JRL   remove_multiplos
        10 => "0111000000000110", -- JRL   remove_multiplos
        11 => "1001000001101111", -- JRL   prox_nao_nulo
        12 => "0111000000001011", -- JRL   prox_nao_nulo
        13 => "0100000000011010", -- CMP   r2, r3
        14 => "0110111111111011", -- BLT   loop1
        15 => "0111000000001101", -- JR    organizar_primos
        -- remove_multiplos: 
        16 => "1000000000001011", -- MOV   r3, r1
        17 => "0111000000000010", -- JR    loop2
        -- remove: 
        18 => "1011000000000001", -- ST.W  r0, 0[r1]
        -- loop2: 
        19 => "0001000000001011", -- ADD   r3, r1
        20 => "0100000000001010", -- CMP   r2, r1
        21 => "0110111111111101", -- BLT   remove
        22 => "1100000000000111", -- JMP   0[r7]
        -- prox_nao_nulo: 
        23 => "0001000000011110", -- ADD  r6, r3
        24 => "1010000000101011", -- LD.W 0[r3], r5
        25 => "0100000000101000", -- CMP r0, r5
        26 => "0101111111111101", -- BE  prox_nao_nulo
        27 => "1100000000000111", -- JMP   0[r7]
        -- organizar_primos: 
        28 => "1001000000010011", -- MOV   2, r3
        29 => "1001000000010001", -- MOV   2, r1
        -- loop3: 
        30 => "1011000000011001", -- ST.W r3, 0[r1]
        31 => "0001000000001110", -- ADD  r6, r1
        32 => "1001000100010111", -- JRL   prox_nao_nulo
        33 => "0111111111110110", -- JRL   prox_nao_nulo
        34 => "0100000000011010", -- CMP  r2, r3
        35 => "0110111111111011", -- BLT  loop3
        -- mostrar_primos: 
        36 => "1001000000010101", -- MOV      2, r5
        -- loop4: 
        37 => "1010000000011101", -- LD.W 0[r5], r3
        38 => "0001000000101110", -- ADD     r6, r5
        39 => "0100000000101001", -- CMP     r1, r5
        40 => "0110111111111101", -- BLT     loop4
        others => (others => '0')
    );

    constant rom_test_jr : mem := (
        0 => "1001000001010001", -- MOV 10, $1
        1 => "0111000000000011", -- JR   3
        2 => "1001000011110011", -- MOV 30, $3
        3 => "0111000000000011", -- JR  3
        4 => "1001000010100010", -- MOV 20, $2
        5 => "0111111111111101", -- JR  -3
        6 => "1001000101000100", -- MOV 40, $4
        others => (others => '0')
    );

    constant rom_test_jmp : mem := (
        0 => "1001000110011001", -- MOV  51, $1
        1 => "1100000000000001", -- JMP   0[$1]
        51 => "1001000010100010", -- MOV  20, $2
        52 => "1100000000101010", -- JMP   5[$2]
        25 => "1001000000101011", -- MOV   5, $3
        26 => "1100001100100011", -- JMP  100[$3]
        others => (others => '0')
    );

    constant rom_test_jrl : mem := (
        0 => "1001000001010001", -- MOV 10, $1
        1 => "1001000010100010", -- MOV 20, $2
        2 => "1001000000100111", -- JRL  add  
        3 => "0111000000000101", -- JRL  add  
        4 => "1001000011110011", -- MOV 30, $3
        5 => "1001000101000100", -- MOV 40, $4
        6 => "1001000110010101", -- MOV 50, $5
        7 => "0111000000000011", -- JR  fim
        8 => "0001000000001010", -- ADD $2, $1  
        9 => "1100000000000111", -- JMP 0[$7] 
        others => (others => '0')
    );
begin
    -- return ROM's content at the input address 
    process(clk)
    begin
        if(rising_edge(clk)) then
            data_out <= rom_primos(to_integer(address));
        end if;
    end process;
end architecture;
